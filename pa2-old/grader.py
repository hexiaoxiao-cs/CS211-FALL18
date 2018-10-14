#!/usr/bin/python
import autograde
import os, os.path

assignment_name = 'PA1'
release = 1

class LifeTests(autograde.FileTests):
    def get_tests(self, prog, data_dir):
        fnames = [fname for fname in os.listdir(data_dir)
            if fname.startswith(self.prefix)
            and fname.endswith(self.ref_suffix)]
        fnames.sort()

        # for each reference name, determine the input file and count
        for ref_name in fnames:
            parts = ref_name[:-len(self.ref_suffix)].split('.')
            steps = parts[-1]
            file_name = '.'.join(parts[:-1]) + self.arg_suffix
            autograde.logger.debug("ref %r steps %s file %r", ref_name, steps, file_name)

            file = os.path.join(data_dir, file_name)
            if not os.path.exists(file):
                autograde.logger.warning('Unmatched reference: %s', ref_name)
                continue

            ref = os.path.join(data_dir, ref_name)

            yield autograde.FileTest(
                cmd=[prog, steps, file],
                input_file=file,
                ref_file=ref)


assignment = autograde.MultiProject(
    autograde.StdinFileTests.Project("llist", weight=4),
    autograde.FileTests.Project("mexp", weight=4),
    autograde.StdinFileTests.Project("stddev", weight=4),
    autograde.Project("bst",
        autograde.StdinFileTests(id="1", weight=4),
        autograde.StdinFileTests(id="2", weight=2),
    ),
    LifeTests.Project("life", weight=2),
    autograde.Project("queens",
        autograde.FileTests(id="1", weight=4),
        autograde.FileTests(id="2", weight=4,
            make_cmd=lambda prog,arg: [prog, "+2", arg]),
        autograde.FileTests(id="3", category=autograde.EXTRA,
            make_cmd=lambda prog,arg: [prog, "-w", arg]),
        autograde.FileTests(id="4", name="3", category=autograde.EXTRA,
            make_cmd=lambda prog,arg: [prog, "+2", "-w", arg]),
        autograde.FileTests(id="1", category=autograde.USER),
        autograde.FileTests(id="2", category=autograde.USER,
            make_cmd=lambda prog,arg: [prog, "+2", arg]),
        autograde.FileTests(id="3", category=autograde.USER,
            make_cmd=lambda prog,arg: [prog, "-w", arg]),
        autograde.FileTests(id="4", category=autograde.USER,
            make_cmd=lambda prog,arg: [prog, "+2", "-w", arg]),
    ),
)

if __name__ == '__main__':
    autograde.set_asan_symbolizer()
    autograde.main(assignment_name, assignment, release)
