#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright (c) 2010 Zurich University of Applied Sciences
# Copyright (c) 2010 Tobias Klauser <tklauser@distanz.ch>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

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
