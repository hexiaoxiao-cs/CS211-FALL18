"""Automated grading of programming assignments.
"""
from __future__ import print_function
import os, os.path, sys
import logging, threading, subprocess, itertools, collections
from contextlib import contextmanager

__author__  = 'David Menendez'
__version__ = '2.2'

logger = logging.getLogger(__name__)

NORMAL, EXTRA, USER = range(3)
category_names = ['Points', 'Extra credit', 'User tests']

class Error(Exception):
    pass

class CommandError(Error):
    def __init__(self, cmd, code, out=None):
        self.cmd = cmd
        self.code = code
        self.out = out

def run_command(cmd):
    """Execute a command without a timeout. Useful for calling make.
    """
    logger.debug('Running %s', cmd)
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    (out,err)= p.communicate()

    if p.returncode != 0:
        raise CommandError(cmd, p.returncode, out)

class Test(object):
    time_limit   = 30
    output_limit = 16*1024
    error_limit  = 10

    def __init__(self, cmd, ref_code=0):
        self.cmd = cmd
        self.ref_code = ref_code

    def handle_stdin(self, proc_stdin):
        proc_stdin.close()

    def reference_output(self):
        return ''

    def compare_output(self, out):
        reflines = self.reference_output().split('\n')
        outlines = out.split('\n')

        errors = [(i,refl,outl) for (i,(refl,outl))
                    in enumerate(itertools.izip(reflines, outlines), 1)
                    if refl != outl]

        if self.error_limit and len(errors) > self.error_limit:
            errs = len(errors) - self.error_limit
            errors = errors[:self.error_limit]
        else:
            errs = 0

        errors = list(itertools.chain.from_iterable(
            ['line {:,}'.format(i),
             '  expected: ' + repr(refl),
             '  received: ' + repr(outl)] for (i,refl,outl) in errors))

        if errs:
            errors.append('{:,} additional errors'.format(errs))

        if len(reflines) < len(outlines):
            errors += ['{:,} extra lines in output'.format(
                len(outlines) - len(reflines))]
        elif len(reflines) > len(outlines):
            errors += [
                'line {:,}'.format(len(outlines)+1),
                '  expected: ' + repr(reflines[len(outlines)]),
                '  received end of file']

        return errors

    def run(self):
        logger.debug('Running %s', self.cmd)
        self.comment = ''

        p = subprocess.Popen(self.cmd,
            stdin  = subprocess.PIPE,
            stdout = subprocess.PIPE,
            stderr = subprocess.STDOUT)

        def cancel():
            p.kill()
            self.comment = 'timed out'

        timer = threading.Timer(self.time_limit, cancel)

        try:
            self.handle_stdin(p.stdin)
            timer.start()
            out = p.stdout.read(self.output_limit)
            if p.stdout.read(1) != '':
                p.kill()
                self.comment = 'exceeded maximum output'

            # make sure we get the final exit code. If we got here, p has
            # either closed stdout or been killed.
            p.wait()
        finally:
            timer.cancel()

        logger.debug('Complete. Code %s\n%s', p.returncode, out)
        self.output = out.rstrip()
        self.returncode = p.returncode
        self.errors = []

        if self.comment:
            return False
        if p.returncode != self.ref_code:
            self.comment = 'unexpected exit code: ' + str(p.returncode)
            self.check_for_sanitizer_output(p.pid)
            return False

        errors = self.compare_output(self.output)
        if errors:
            self.comment = 'incorrect output'
            self.errors = errors
            return False

        self.comment = 'correct'
        return True

    def check_for_sanitizer_output(self, pid):
        """Look for a summary from AddressSanitizer or similar.
        """

        keyword = '=={}=='.format(pid)
        logger.debug('Checking for %r', keyword)

        lines = iter(self.output.split('\n'))
        for line in lines:
            if line.startswith(keyword):
                if 'AddressSanitizer' in line:
                    self.comment = 'terminated by AddressSanitizer'
                break
        else: # not found
            return

        # continue searching for SUMMARY
        for line in lines:
            if line.startswith('SUMMARY:'):
                self.errors = [line]
                return

    def print_output(self):
        if not hasattr(self, 'output'):
            raise Exception('print_output called before run')

        print()
        print('output')
        print('------')
        print(self.output)
        print('------')

    def print_input(self):
        pass


class RefTest(Test):
    """Test comparing the program output with a given reference string.
    """
    def __init__(self, cmd, ref, **kws):
        super(RefTest, self).__init__(cmd, **kws)
        self.ref = ref

    def reference_output(self):
        return self.ref

class FileRefTest(Test):
    """Test comparing the program output to an output specified by
    ref_file.
    """
    def __init__(self, cmd, ref_file, **kws):
        super(FileRefTest, self).__init__(cmd, **kws)
        self.ref_file = ref_file

    def reference_output(self):
        try:
            logger.debug('Opening reference file %r', self.ref_file)
            return open(self.ref_file).read().rstrip()
        except IOError as e:
            raise Error('Unable to open reference file {!r}: {}'.format(
                self.ref_file, e.strerror))

    def run(self):
        correct = super(FileRefTest,self).run()
        if not correct and self.returncode == 0:
            self.errors.insert(0, 'reference file: ' + repr(self.ref_file))

        return correct

class FileInputTest(Test):
    """Test with a specified input given by input_file.

    Note: the test itself does not send the input to the program. It should be
    passed to the program as an argument.
    """
    def __init__(self, cmd, input_file, **kws):
        super(FileInputTest, self).__init__(cmd, **kws)
        self.input_file = input_file

    def print_input(self):
        try:
            logger.debug('Opening input file %r', self.input_file)
            input = open(self.input_file).read().rstrip()

            print()
            print('input')
            print('-----')
            print(input)
            print('-----')

        except IOError as e:
            raise Error('Unable to open input file {}: {}'.format(
                self.input_file, e.strerror))

class FileStdinTest(FileInputTest):
    """Test where the input specified by input_file is written to the program's
    standard input.
    """
    def handle_stdin(self, stdin):
        try:
            logger.debug('Opening input file %r', self.input_file)
            with open(self.input_file) as f:
                stdin.write(f.read())
        except IOError as e:
            raise Error('Unable to open input file {!r}: {}'.format(
                self.ref_file, e.strerror))
        finally:
            stdin.close()

    def run(self):
        correct = super(FileStdinTest, self).run()
        if not correct:
            self.errors.insert(0, 'input file: ' + repr(self.input_file))

        return correct

class FileTest(FileRefTest, FileInputTest):
    pass

class StdinFileTest(FileStdinTest, FileTest):
    pass

class AbstractTestGroup(object):
    @classmethod
    def Project(cls, name, *args, **kws):
        tests = cls(*args, **kws)
        return Project(name, tests)

    def __init__(self, id=None, weight=1, name=None, category=NORMAL,
            make_cmd=None):
        self.id = id
        self.name = name or id
        self.weight = weight
        self.category = category

        if make_cmd:
            self.make_cmd = make_cmd

    def get_name(self, project_name):
        if self.name is None:
            return project_name

        return project_name + ':' + self.name

    def get_tests(self, prog, data_dir):
        raise NotImplementedError

    @staticmethod
    def make_cmd(prog, arg):
        return [prog, arg]

class StringTests(AbstractTestGroup):
    """Look for tests in a file named <prefix><id><suffix>.
    """
    def __init__(self, prefix='tests', suffix='.txt', **kws):
        super(StringTests, self).__init__(**kws)
        self.file = prefix + (self.id or '') + suffix

    def get_tests(self, prog, data_dir):
        test_file = os.path.join(data_dir, self.file)

        if not os.path.exists(test_file):
            logger.warning('Test file not found: %r', test_file)
            return

        logger.debug('Opening tests file: %r', test_file)

        try:
            lines = open(test_file)

            while True:
                arg = next(lines).rstrip()
                ref = next(lines).rstrip()

                yield self.Test(cmd=self.make_cmd(prog, arg), ref=ref)

        except StopIteration:
            return

    Test = RefTest

class FileTests(AbstractTestGroup):
    """Look for pairs of test files containin the input and reference output.

    If id is None, they are named:
        <prefix><test><arg_suffix>
        <prefix><test><ref_suffix>

    Otherwise, they are named:
        <prefix><id>.<test><arg_suffix>
        <prefix><id>.<test><ref_suffix>
    """
    def __init__(self, prefix='test.', arg_suffix='.txt', ref_suffix='.ref.txt',
            **kws):
        super(FileTests, self).__init__(**kws)
        self.arg_suffix = arg_suffix
        self.ref_suffix = ref_suffix

        if self.id:
            self.prefix = prefix + self.id + '.'
        else:
            self.prefix = prefix

    def get_tests(self, prog, data_dir):
        # gather the names of the reference files in data_dir
        fnames = [fname for fname in os.listdir(data_dir)
                    if fname.startswith(self.prefix)
                    and fname.endswith(self.ref_suffix)]
        fnames.sort()

        # for each reference name, check for the corresponding input file
        for ref_name in fnames:
            # swap ref_suffix for input suffix
            arg_name = ref_name[:-len(self.ref_suffix)] + self.arg_suffix
            arg = os.path.join(data_dir, arg_name)

            # skip this name if no input file exists
            if not os.path.exists(arg):
                logger.warning('Unmatched reference file: %s', ref_name)
                continue

            ref = os.path.join(data_dir, ref_name)

            yield self.Test(
                cmd = self.make_cmd(prog, arg),
                input_file = arg,
                ref_file = ref)

    Test = FileTest

class StdinFileTests(FileTests):
    Test = StdinFileTest

    @staticmethod
    def make_cmd(prog, arg):
        return [prog]



class Project(object):
    def __init__(self, name, *groups, **kws):
        self.name = name
        self.prog = kws.get('prog_name', name)

        if not groups:
            raise ValueError('Must provide at least one test group')

        # separate graded and ungraded test groups
        self.groups = tuple(g for g in groups if g.category != USER)
        self.user_groups = tuple(g for g in groups if g.category == USER)

        # make sure the groups have distinct ids
        groupids = collections.Counter(g.id for g in self.groups)
        if len(groupids) < len(self.groups):
            raise ValueError('Duplicate test group ids: ' +
                    str([g for g in groupids if groupids[g] > 1]))

        # make sure the user groups have distinct ids
        groupids = collections.Counter(g.id for g in self.user_groups)
        if len(groupids) < len(self.user_groups):
            raise ValueError('Duplicate test group ids: ' +
                    str([g for g in groupids if groupids[g] > 1]))

        # generate a user test group if none provided and not disabled
        if not self.user_groups:
            user_class = kws.get('user_class', type(self.groups[0]))
            if user_class is not None:
                self.user_groups = (user_class(category=USER),)


    def build(self):
        run_command(['make', 'clean'])

        if os.path.exists(self.prog):
            raise Error('not removed by "make clean"')

        run_command(['make', self.prog])

        if not os.path.exists(self.prog):
            raise Error('not created by "make"')


    def test_project(self, runner, src_dir, data_dir, user_dir='tests'):
        logger.info('Grading %r', self.name)

        # check for src_dir
        if not os.path.isdir(src_dir):
            logger.info('Source dir not found: %r', src_dir)
            return

        # check for data_dir
        if not os.path.isdir(data_dir):
            raise Error('Data directory not found: ' + repr(data_dir))

        prog = './' + self.prog

        with runner.project_context(self.name):
            os.chdir(src_dir)
            self.build()

            for group in self.groups:
                name = group.get_name(self.name)

                if runner.begin_group(name, group.category, group.weight):
                    for test in group.get_tests(prog, data_dir):
                        runner.run_test(test)

                    runner.record_group()

            if self.user_groups and os.path.isdir(user_dir):
                for group in self.user_groups:
                    name = group.get_name(self.name)

                    if runner.begin_group(name, group.category, group.weight):
                        for test in group.get_tests(prog, user_dir):
                            runner.run_test(test)

                        runner.record_group()




class MultiProject(object):
    def __init__(self, *projects):
        self.projects = projects

    def test_project(self, runner, src_dir, data_dir, **kws):
        for p in self.projects:
            src  = os.path.join(src_dir, p.name)
            data = os.path.join(data_dir, p.name)

            p.test_project(runner, src, data, **kws)


class TestRunner(object):
    def __init__(self):
        self.scores = collections.defaultdict(list)

        self.current_project = ''
        self.current_group = ''
        self.category = NORMAL
        self.weight = 1.0
        self.tested = 0
        self.successes = 0
        self.failures = 0
        self.errors = 0

        # TODO these should be parameters to the constructor
        self.categories = set((NORMAL, USER))
        self.requests = ()
        self.project_weights = {}
        self.suppress_successes = True
        self.show_errors = True
        self.show_input = True
        self.show_output = True

        self.show_status = True
        self.bar_visible = False

    @contextmanager
    def project_context(self, name):
        self.current_project = name
        self.current_scores = collections.defaultdict(collections.Counter)

        try:
            yield

        except EnvironmentError as e:
            self.errors += 1

            self.clear_status()
            print()
            print('{0}: {1}'.format(name, e.strerror))
            if e.filename:
                print(' ', e.filename)

        except CommandError as e:
            self.errors += 1

            self.clear_status()
            print()
            print('{0}: error running {1.cmd[0]!r} (return code {1.code})'.format(name, e))
            if len(e.cmd) > 1:
                print('  arguments:', e.cmd[1:])
            if e.out is not None:
                print(e.out)

        except Error as e:
            self.errors += 1

            logger.info('%s failed: %s', name, e)
            self.clear_status()
            print()
            print('{0}: {1}'.format(name, e))

        finally:
            # record the scores for this project
            logger.debug('Gathering scores for %s', self.current_project)

            weight = self.project_weights.get(name, 1) * \
                    self.project_weights.get('.', 1)

            for group in sorted(self.current_scores.keys()):
                for cat, score in self.current_scores[group].iteritems():
                    self.scores[cat].append((group, score * weight))

            # clear the current project
            self.current_project = ''


    def clear_status(self):
        if self.bar_visible:
            sys.stderr.write('\r')
            sys.stderr.write(' ' * 80)
            sys.stderr.write('\r')
            self.bar_visible = False

    def write_status(self):
        if self.show_status:
            status = '\rDone {:3} Failed {:3} Errors {:3}'.format(
                self.tested, self.failures, self.errors)
            sys.stderr.write(status)
            self.bar_visible = True


    def finish(self):
        self.clear_status()

        print()
        print('Tests performed {0.tested:3}\n'
              'Tests failed    {0.failures:3}'.format(self))
        if self.errors:
            print('Errors          {:3}'.format(self.errors))


        for category,catscores in self.scores.iteritems():
            total_score = 0

            # get longest group name
            gr_width = max(5, max(len(i[0]) for i in catscores))


            print()
            print(category_names[category])
            print('{0:{1}} Score'.format(' ', gr_width))

            total = 0
            for (group, score) in catscores:
                print('{0:{1}} {2:5.1f}'.format(group, gr_width, score))
                total += score

            if len(catscores) > 1:
                print('-' * gr_width, '-----')
                print('{0:{1}} {2:5.1f}'.format('Total', gr_width, total))


    def begin_group(self, name, category, weight):
        if category not in self.categories:
            return False

        if self.requests and self.current_project not in self.requests \
                and name not in self.requests:
            return False

        self.current_group = name
        self.category = category
        self.weight = weight
        self.successes = 0

        return True

    def record_group(self):
        if not self.current_group:
            raise Exception('record_group called before begin_group')

        self.current_scores[self.current_group][self.category] += \
            self.successes * self.weight

    def run_test(self, test):
        self.tested += 1

        if test.run():
            self.successes += 1

            if self.suppress_successes:
                self.write_status()
                return

        else:
            self.failures += 1

        self.clear_status()
        print()
        print('{0}: {1}'.format(self.current_project, test.comment))
        print('  called as', test.cmd)

        if self.show_errors:
            for line in test.errors:
                print(' ', line)

        if self.show_input:
            test.print_input()

        if self.show_output:
            test.print_output()

        self.write_status()

@contextmanager
def temp_dir():
    """Create a temporary directory, and delete it and its contents once
    the context has been closed. Yields the directory path
    """
    import tempfile, shutil

    dir = tempfile.mkdtemp()
    try:
        logger.debug('Created temporary directory: %r', dir)
        yield dir

    finally:
        logger.debug('Deleting temporary directory')
        shutil.rmtree(dir)


logcfg = {
    'version': 1,
    'disable_existing_loggers': False,
    'formatters': {
        'normal': { 'format': '%(asctime)s %(levelname)-8s %(message)s' },
    },
    'handlers': {
        'file': {
            'class': 'logging.FileHandler',
            #'filename': 'autograder.log',
            'filename': os.path.join(sys.path[0], 'autograder.log'),
            'mode': 'a',
            'formatter': 'normal',
            'delay': True,
        },
    },
    'root': {
        'handlers': ['file'],
    },
}

def main(name, assignment, release=1,
        src_subdir='src',
        data_subdir='data',
        logcfg=logcfg):
    import argparse, logging.config, tempfile
    argp = argparse.ArgumentParser()
    argp.add_argument('-v', '--verbose', action='count', default=0,
        help='Print more output')
    argp.add_argument('-q', '--quiet', action='count', default=0,
        help='Print less output')
    argp.add_argument('-s', '--src', metavar='dir', default=src_subdir,
        help='Directory containing program files')
    argp.add_argument('-a', '--archive', metavar='tar',
        help='Archive containing program files (overrides -s)')
    argp.add_argument('-x', '--extra', action='store_true',
        help='Include extra credit tests')
    argp.add_argument('-m', '--multiply', nargs=2, metavar=('project','factor'),
        action='append', default=[],
        help='Multiply a particular project score by some factor.')
    argp.add_argument('-d', '--debug', action='store_true',
        help='Increase logging')
    argp.add_argument('program', nargs='*',
        help='Name of program to grade')

    args = argp.parse_args()

    if logcfg:
        logging.config.dictConfig(logcfg)

    if args.debug:
        logger.setLevel(logging.DEBUG)

    logger.info('Starting autograder %s release %s. Library %s',
        name, release, __version__)

    # data directory is relative to grader
    data_dir = os.path.join(sys.path[0], data_subdir)

    logger.debug('Data directory: %r', data_dir)

    # set up the test runner
    # TODO: improve this interface
    runner = TestRunner()
    runner.requests = set(args.program)
    if args.extra:
        runner.categories.add(EXTRA)

    verb = args.verbose - args.quiet
    if verb < 0:
        runner.show_errors = False
    if verb < 1:
        runner.show_input = False
        runner.show_output = False
    if verb > 1:
        runner.suppress_successes = False

    for p,w in args.multiply:
        try:
            runner.project_weights[p] = float(w)
        except ValueError:
            print("Factor for {!r} must be a number".format(p), file=sys.stderr)
            exit(1)

    try:
        print(name, 'Auto-grader, Release', release)

        if args.archive:
            archive = os.path.realpath(args.archive)
            logger.debug('Archive path: %r', archive)

            if not os.path.exists(archive):
                raise Error('archive not found: ' + repr(archive))

            with temp_dir() as dir:
                os.chdir(dir)
                run_command(['tar', 'xf', archive])

                if not os.path.isdir(src_subdir):
                    raise Error('archive does not contain directory ' +
                        repr(src_subdir))

                src_dir = os.path.realpath(src_subdir)

                assignment.test_project(runner, src_dir, data_dir)


        else:
            src_dir = os.path.realpath(args.src)

            logger.debug('Source directory: %r', src_dir)

            if not os.path.isdir(src_dir):
                raise Error('invalid src directory: ' + repr(src_dir))

            assignment.test_project(runner, src_dir, data_dir)

        runner.finish()

    except Error as e:
        runner.clear_status()
        print("grader:", e)
        exit(1)
    except Exception as e:
        logger.exception('Uncaught exception: %s', e)
        runner.clear_status()
        print("grader: internal error")
        exit(1)


def set_asan_symbolizer(symbolizer='llvm-symbolizer'):
    from distutils.spawn import find_executable

    if 'ASAN_OPTIONS' in os.environ or 'ASAN_SYMBOLIZER_PATH' in os.environ:
        return

    path = find_executable(symbolizer)
    if path is None:
        return

    logger.info('Setting ASAN_SYMBOLIZER_PATH=%s', path)
    os.environ['ASAN_OPTIONS'] = 'symbolize=1'
    os.environ['ASAN_SYMBOLIZER_PATH'] = path
