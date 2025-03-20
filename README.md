# EASY64
## **E**asy and **A**wesome **S**cripting for **Y**ou!  **64**bit
Spelled EASY64, Easy64, EASY, Easy
## Statement of Futility
* I don't need a new programming language; very few people do.
* I don't need to learn how to design programming languages; this does absolutely nothing by my career.
* I am the victim of urges I can barely contain; Please send help.
## Statement of Purpose
* A typed, interpreted scripting language written in C with the minimum working structure to be Turing Complete and Interesting to me.
* The sequel to [SPARROW](https://github.com/jwatson-CO-edu/SPARROW), which was a dynamic, interpreted [Scheme](https://mitpress.mit.edu/9780262560993/the-little-schemer/) variant written in [D](https://dlang.org/).
* This is for my own entertainment and edification.  
* No claims of utility or functionality are made or implied.

# Design Principles
* Simple & Ergonomic
* Typed
* Interpreted
* Only 64-bit primitive types offered {`double`, `long int`, `unsigned long int`}
* Interpreter with REPL and file execution capabilities
* Component System Units: Favor Composition Over Inheritance
    - Unit = Components (Members) + System (Methods)
        * A semi-contained struct that is meant to be part of a larger entity
        * Defines a certain combination of components
        * Defines methods that operate on the specified components; the system. When the system follows the unit, so do capabilities associated with the unit
    - Also allow the class itself to define methods
* Inheritance supported but discouraged! 

# Specification
## Types
### Primitives
* `float` (double)
* `int` (long)
* `uint` (unsigned long)
### Code
* How to handle code as a first-class type?
### Composites
* Arrays (zero-based)
* Strings
### User-Defined Types: Component System Units
* ???
### Control Flow
* Idea: All conditionals can be their **own code blocks** so that _bookkeeping_ and _computation_ can be **kept separate if desired**!
    - All the codeblocks in the same conditional share a scope, They are (kind of) the same code block under the hood
    - Conditionals are regular blocks of code, the truth of which is determined by `eval`, If the keyword is omitted, then the truth of the last statement is considered the result
    - `while{}{}`
    - `do{}`-`while{}`
    - `for{}`
    - `if{}`-`then{}`-`else{}`
        * `if`: Conditional
        * `then`: Block executed if `eval` is true-like
        * `else`: Block executed if `eval` is false-like
* Also offer the traditional versions
    - `for()`
        * Traditional
        * Iterator

# Implementation
## Lexer
* Warning: Do NOT generate substrings BEFORE lexing!
* Generate a Queue of AST nodes
* Reserved tokens ALWAYS stand on their own and are NEVER part of an identifier
* Reserved words CAN stand on their own ONLY when they are NOT part of an identifier
## Parser
* Parser case handlers are called "parslets"
* Each parslet should provide a cue of how to fill in needed data

# `DEV PLAN`
* `[>]` Basic Math Expressions
    - `[>]` Numeric data types, Simplest possible, using [`std::variant`](https://en.cppreference.com/w/cpp/utility/variant)
    - `[>]` Math calculations, stack model
* `[ ]` Tree parser
    - Expectations about future branches?
* `[ ]` String representation of `AST_Node`
* `[ ]` String representation of AST
* `[ ]` Parser probably needs a FSM as SPARROW had!
    - `[ ]` Each parslet returns {Next Mode?, Pointer to where the next node is needed?}
    - `[ ]` Keeps track of what token to parse next?
    - `[ ]` Job stack?
    - `[ ]` State struct?
* `[ ]` Add line number to `AST_Node` for user debugging

# Future Possibilities
* How does the interpreter keep from defining a variable multiple times? - Just check if it exists whenever the statement is run!
* Primitive Type: Bit Block - 64 addressable booleans
* Domains
    - 3D Turtle Mode
    - Native Behavior Trees
    - Native Geometric Algebra
    - Deep Learning / QDN
    - Machine Learning
* GPU Compute backend
    - OpenGL?: Do this if graphics come first 
    - OpenCL?: Do this for a scientific/distributed computing context
* Transpilation to C/++ --> Compilation pipeline
* Piles and piles of absolute, frothing nonsense (See below sections).

# Bryozoa: Networked Flow Programming Language (DANGER: This is insane!)
* A process as a first-class object
* Do I need a `synchronized` keyword like Java?
* Imagine using the Bash pipe operator, but as a language
* Parallel Model: Hosts & Jobs
    - Host: EASY interpreter instance
        * One Instance, One Process
        * No Sharing: Must copy all necessary data to run the job
        * No control of the job once spawned
    - Job: Struct that both holds work definition, work status, and computation result 
        * Main program can check a job to confirm that the result is ready
        * Main program can wait for a job to be ready if desired
        * Idea: Jobs can be updated more than once
            - Tagged with freshness metadata
            - Main program has choice to wait for update or to proceed with stale data
* [Communication Between Processes on Same Machine](https://stackoverflow.com/questions/670891/is-there-a-way-for-multiple-processes-to-share-a-listening-socket)
    - [Unix Domain Socket (UDS)](https://en.wikipedia.org/wiki/Unix_domain_socket#Socket_instantiation)
    - [UDS Example](https://medium.com/swlh/getting-started-with-unix-domain-sockets-4472c0db4eb1)
    - [Shared Memory Between Processes](https://man7.org/linux/man-pages/man7/shm_overview.7.html)
        * [`shm_open` Example](https://gist.github.com/garcia556/8231e844a90457c99cc72e5add8388e4)
* [Communication Between Hosts on a Local Network](https://claude.site/artifacts/f7b0532a-e795-4a3d-976c-06ae89ea4c83)

# Just, Positively Insane Ideas
* EASY64 as a creative coding / visualization console
* Make Bryozoa with the components needed to rapidly define cognitive architectures
    - Call Python programs (including Tensorflow/PyTorch) as sub-processes
