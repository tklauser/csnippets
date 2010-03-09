#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys
import getopt
import subprocess

class colors:
    RED = '\033[91m'
    GREEN = '\033[92m'
    END = '\033[0m'

    def disable(self):
        self.RED = ''
        self.GREEN = ''
        self.END = ''

    def red(self, str):
        return self.RED + str + self.END

    def green(self, str):
        return self.GREEN + str + self.END

failed = passed = 0
c = colors()

def fail(t, ret):
	global failed, c
	print(" %s %s (%s)" % (c.red("[FAIL]"), t, ret))
	failed += 1

def ok(t):
	global passed, c
	print(" %s %s" % (c.green("[PASS]"), t))
	passed += 1

def main():
	if len(sys.argv) < 2:
		sys.exit(0)

	total = len(sys.argv[1:])

	for t in sys.argv[1:]:
		t_p = os.path.join('.', t)
		if not os.path.exists(t_p):
			fail(t, "not found")
			continue

		ret = subprocess.call(t_p)
		if ret == 0:
			ok(t)
		else:
			fail(t, "retval %s" % ret)

	print("Ran %s tests (%s, %s)" % (total, c.green(str(passed) + " passed"), c.red(str(failed) + " failed")))
	exit(ret)

if __name__ == '__main__':
	main()
