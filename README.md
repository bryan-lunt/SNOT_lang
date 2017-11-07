# SNOT_lang
Simple Numerical Object Transcription (SNOT) is a language similar to JSON meant for numerical computing.

## PURPOSE

Nearly all optimizers expect parameters in a vector, array, or list type.
They expect to optimize a scalar valued function with a vector input.

`val = func(x_0,x_1,x_2,...; some_other_non_parameter_arguments)`

usually as:

`val = func(double[] in ; some_other_non_parameter_arguments)`

However, when functions or models become more complicated,
it may become difficult for programmers to keep track of where in the array each argument is.
This is _much_ more difficult when the program in question creates the model/function dynamically.
In these cases, it may simply not be known in advance which position in the array contains which argument.
It needs to be found dynamically at runtime.

It would be very nice if the code within func(...) can access/view the parameters in a meaningful hierarchical structure.

In the experience of this author, people end up rolling their own hard-coded datatypes / data structures for this.
This can become unwieldy, and it may make future maintenance difficult, in particular when new parameters are introduced.

It would be nice to create the structure dynamically (think like Python dictionaries/lists or a JSON object.) then be able to move back and forth between an easily viewable/usable hierarchical structure and a vector representation.
Code within the objective function can be written cleanly, while the optimizer gets parameters in its preferred view.

## THIS PROJECT

This project implements a C++98 compliant, header-only dynamic data structure that addresses precisely this problem.
The code is licensed under an MIT license, so you can simply copy the header `param_storage.h` into your optimization project and use it.

The API is largely STL/C++ like, with an object `gsparams::DictList` that can store one of three things:

* A primitive value. This can currently only be a double, but you can change the typedef and use whatever, including a struct or union.
* A list. The list can contain any of these three types.
* A dictionary mapping std::string to values (any of these three types.). This dictionary preserves the order in which objects are added to it.


The key thing is that the dictionary preserves the order. This allows traversals to go between vector and structured representation.
A traversal and an iterator are implemented/included.

## EXAMPLES

You can create a structure dynamically.

```C++
    DictList top_dict;

    DictList tfs;

    DictList a_tf;
    DictList b_tf;

    a_tf.push_back(1.23);
    a_tf.push_back(1.24);
    a_tf.push_back(1.25);
    tfs["A"] = a_tf;

    b_tf.push_back(2.1);
    b_tf.push_back(2.2);
    b_tf.push_back(2.3);


    tfs["B"] = b_tf;

    top_dict["tfs"] = tfs;
```

Which is a structure equivalent to:

```JSON
{"tfs":{
    "A":[1.23,1.24,1.25],
    "B":[2.1,2.2,2.3]
    }
}
```

Now, you can access values out of that like this:

```C++
    top_dict["tfs"]["A"][0] = 1.2345; //Should only do this once it has a zeroth element.

    double foobar = 5.0*top_dict["tfs"]["A"][0];
    dictlist_primitive_t foobaz = 3.3/top_dict["tfs"]["A"][0];
```

Here's an example (from `example1.cpp`) where it is used inside a Rosenbrock function. It is just a simple example, but you can imagine functions with a much more complicated set of parameters.

```C++
return pow(a-tmpvals["x"],2.0) + b*pow(tmpvals["y"]-pow(tmpvals["x"],2.0),2.0);
```

Of course you could convert them to something more static if you feel that the subscripting is too slow, perhaps in very tight-looped numerical functions. (However, a branch predicting complier probably makes this unnecessary. Don't optimize too early!)

```C++
    double x = tmpvals["x"];
    double y = tmpvals["y"];

    return some_iterated_function_that_uses_x_and_y_many_times(x,y);
```


#### Bibliography

The SNOT grammar was adapted from the JSON grammer available here: https://gist.github.com/justjkk/436828/

I benefit from this tutorial: https://www.usna.edu/Users/cs/lmcdowel/courses/si413/F10/labs/L04/calc1/ex1.html
(and parts of the code come from there, with heavy modifications.)

In future versions, I hope to benefit from this tutorial and make the grammer create better C++ code.
https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html

https://github.com/linse/flex-bison-cpp-example

https://www.gnu.org/software/automake/manual/html_node/Yacc-and-Lex.html

The ANTLR4 grammar was cut down from the JSON grammar at https://raw.githubusercontent.com/antlr/grammars-v4/master/json/JSON.g4
