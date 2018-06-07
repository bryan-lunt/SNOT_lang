from __future__ import unicode_literals

__license__ = """MIT License

Copyright (c) 2017 Bryan Lunt <bjlunt2@illinois.edu> <bryan.j.lunt@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""


import sys as _sys

import numpy as _np
import json as _J
from collections import OrderedDict as _OD

from itertools import count as _count

#### VERSION DEPENDENT
iteritems_attr = None
if _sys.version_info[0] < 3:
	from itertools import izip as _izip
	iteritems_attr = "iteritems"
else:
	_izip = zip
	from functools import reduce
	iteritems_attr = "items"

def load(filelike):
	return _J.load(filelike,object_pairs_hook=_OD,parse_int=lambda x:float(x))

def loads(stringlike):
	return _J.loads(stringlike,object_pairs_hook=_OD,parse_int=lambda x:float(x))

from json import dump,dumps

def _traverse_py(in_snot):
    def join_lists(a,b):
        return a+b
    
    if hasattr(in_snot,"values"):
        return reduce(join_lists,[_traverse_py(i) for i in in_snot.values()])
    elif isinstance(in_snot,list):
        return reduce(join_lists,[_traverse_py(i) for i in in_snot])
    else:
        return [in_snot]

def traverse(in_snot):
	return _np.array(_traverse_py(in_snot))


def _populate_py(in_snot,in_vector,begin=0):
	num_consumed = 0
	the_iter = None
	if hasattr(in_snot,iteritems_attr):
		the_iter = getattr(in_snot,iteritems_attr)();
	elif isinstance(in_snot,list):
		the_iter = _izip(_count(),in_snot)
	else:
		pass#should not descend into scalars.

	for key,val in the_iter:
		if hasattr(val, iteritems_attr) or isinstance(val,list):
			num_consumed += _populate_py(val, in_vector,begin+num_consumed)
		else:
			in_snot[key] = in_vector[begin+num_consumed]
			num_consumed+=1
	return num_consumed

def populate(in_snot,in_vector):
	return _populate_py(in_snot,in_vector)

def paths(in_snot,base_path="$"):
	def join_lists(a,b):
		return a+b

	if hasattr(in_snot,iteritems_attr):
		return reduce(join_lists,[paths(v,base_path+"["+k+"]") for k,v in getattr(in_snot,iteritems_attr)()])
	elif isinstance(in_snot,list):
		return reduce(join_lists,[paths(v,base_path+"["+str(k)+"]") for k,v in _izip(_count(),in_snot)])
	else:
		return [str(base_path)]


__all__ = ["load","loads","dump","dumps","traverse","populate","__license__"]
