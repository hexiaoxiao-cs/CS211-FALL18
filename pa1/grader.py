#!/usr/bin/python
import autograde
import os, os.path

assignment_name = 'PA1'
release = 1

class MultiStringTests(autograde.StringTests):
    def make_test(self, prog, arg, ref):
        return self.Test(cmd=[prog] + arg.split(), ref=ref)

assignment = autograde.MultiProject(
    MultiStringTests.Project('gcd', weight=3),
    autograde.StringTests.Project('rot13', weight=3),
    autograde.StringTests.Project('rle', weight=3),
)

if __name__ == '__main__':
    autograde.set_asan_symbolizer()
    autograde.main(assignment_name, assignment, release)
