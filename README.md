# EASY64
## **E**asy and **A**wesome **S**cripting for **Y**ou!  **64**bit
Spelled EASY64, Easy64, EASY, Easy
## Statement of Futility
* I don't need a new programming language; very few people do.
* I don't need to learn how to design programming languages; this does absolutely nothing by my career.
* I am the victim of urges I can barely contain; Please send help.
## Statement of Purpose
* A typed, interpreted scripting language written in C++ with the minimum working structure to be Turing Complete and Interesting to me.
* The sequel to [SPARROW](https://github.com/jwatson-CO-edu/SPARROW), which was a dynamic, interpreted [Scheme](https://mitpress.mit.edu/9780262560993/the-little-schemer/) variant written in [D](https://dlang.org/).
* This is for my own entertainment and edification.  
* No claims of utility or functionality are made or implied.

# Design Principles
* Simple & Ergonomic
* Typed
* Interpreted
* Only 64-bit primitive types offered {`double`, `long int`, `unsigned long int`}
* Interpreter with REPL and file execution capabilities

## Future Possibilities
* Parallel Model: Jobs & Places
    - Jobs: EASY interpreter running in a separate, asynchronous process
        * No Sharing: Must copy all necessary data to run the job
        * No control of the job once spawned
    - Places: Structs that both hold the job result and the status of the job
        * Main program can check a place to confirm that the result is ready
        * Main program can wait for a place to be ready if desired
        * Idea: Places can be updated more than once
            - Tagged with freshness metadata
            - Main program has choice to wait for update or to proceed with stale data
* Transpilation to C++
* Flow programming
* Piles and piles of absolute, frothing nonsense

# Implementation
## Lexer
* Lexer returns a vector of string tokens to be parsed
* Reserved tokens ALWAYS stand on their own and are NEVER part of an identifier
* Reserved words CAN stand on their own ONLY when they are NOT part of an identifier
## Parser
* Parser case handlers are called "parslets"
* Each parslet should provide a cue of how to fill in needed data

# `DEV PLAN`
* `[Y]` Numeric data types, 2023-08-26: Simplest possible, using unions
* `[>]` Math calculations, stack model
* `[ ]` Tree parser
* `[ ]` String representation of `AST_Node`
* `[ ]` String representation of AST
* `[ ]` Parser probably needs a FSM as SPARROW had!
    - `[ ]` Each parslet returns {Next Mode?, Pointer to where the next node is needed?}
    - `[ ]` Keeps track of what token to parse next?
    - `[ ]` Job stack?
    - `[ ]` State struct?
* `[ ]` Add line number to `AST_Node` for user debugging