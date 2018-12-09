#!/usr/bin/python
import autograde
import os, os.path

assignment_name = 'PA5'
release='1'

class CacheTests(autograde.FileTests):
    def __init__(self, ref_suffix='.txt', **kws):
        super(CacheTests, self).__init__(ref_suffix=ref_suffix, **kws)

        repl,pref = kws['id'].split('.')

        self.repl = repl
        self.pref = pref

    def get_tests(self, prog, data_dir):

        # gather the names of the reference files
        fnames = [fname for fname in os.listdir(data_dir)
                    if fname.startswith(self.prefix)
                    and fname.endswith(self.ref_suffix)]
        fnames.sort()

        autograde.logger.debug('Tests for %s: %s', self.id, fnames)

        # for each reference name, determine the corresponding trace file
        for ref_name in fnames:
            id = ref_name[len(self.prefix):-len(self.ref_suffix)]
            trace, csize, assoc, bsize = id.split('.')

            tracefile = os.path.join(data_dir,
                'trace' + trace + '.txt')

            if not os.path.exists(tracefile):
                autograde.logger.warning('Unmatched reference file: %r', ref_name)
                continue

            ref = os.path.join(data_dir, ref_name)

            yield autograde.FileRefTest(
                cmd = [prog, csize, assoc, self.pref, self.repl, bsize, tracefile],
                ref_file = ref,
            )

assignment = autograde.Project('cachesim',
    CacheTests(id='fifo.p0', weight=2.5),
    CacheTests(id='fifo.p1', weight=2.5),
    CacheTests(id='lru.p0', category=autograde.EXTRA),
    CacheTests(id='lru.p1', category=autograde.EXTRA),
    user_class=None,
)



if __name__ == '__main__':
    autograde.set_asan_symbolizer()
    autograde.main(assignment_name, assignment, release)
