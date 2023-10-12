Design Plan:

The design plan was guided by the psuedocode and the rubric set. First was building out the functionalities of all the builtins. Followed was the ability to parse through the environment variable to find the command that was requested. Redirection, Piping, and Background processing followed in that order.

Using existing knowledge helped to make the psuedocode, so specifics of functionality were not as heavily researched, but the flow of the program was understood for the most part. Along every step of the process, most functions and their purpose were researched.

The design is split into main 4 files: The shell driver, the builtin library, the extern_library that would handle any command thats not within the builtin library, and the provided helpers library. This was the main way of separation as these seemed to be the biggest macro purposes, while still not having too much within each file.
The shell driver program is the main program that holds all the other files together. In it is an infinite loop to continue asking for prompts from the user that would be executed through the other three files aforementioned. In the driver, it processed which file was best suited to access that would fit the prompt of the user.
The builtin library contains the five methods that were asked from the project. These functions do not fork() and just simply complete themselves within the driver and all have no return values. The help function specifically calls exec onto the more program as it is a file that contains many lines of documentation. The documentation is from the help.txt files, and lists the capabilities of the mock shell. All the other builtins behave exactly as they would within the bash.
The extern_library deals with the redirection and piping as well as parsing through the environment variable as these functionalities are accessed through commands that are not builtins (taken from the linux library). The parsing is done by tokenizing the environment variable and search within the directory if the user prompted command is in there. if it is, build out the path and return it. The redirection is done through changing of file descriptors and execing the first command that is prompted by the user. The redirection handles either < or > and the ability to handle both in the same line. It does not logically support multiple of the same redirection. Piping handles indefinite piping by creating child processes for each command. The file descriptors are laid out in the form of a 2D array as it was logically easier for me to understand, rather than 1D array. The 2D array allows for the first "dimension" to be the amount of pipes needed and the second "dimension" refers to either the input or output side of the pipe (0 or 1).
The helpers library provides the parse of the command line and the ability to find the characters for redirection, piping, and background processing.



Testing Plan:

The majority of the testing was done in a separate file for unit testing. Every single function that was built and subset purposes within them were all isolated using different inputs and testing for outputs. Once the function seemed to function correctly, it was tested within the main driver shell program to see if there were any weird interactions that could occur due to the program flow. 

Since redirection was a big part of this program, print statements were not a feasable option to see if a function was behaving correctly. A lot of the times I had to isolate a portion that I believed to be the problem to see if it was. An example that served me a lot of time to fix on multiple occasions was the manipulation of the entire char **commandline, to see if the it was fully built out correctly, including the shifting of elements, or changing to nulls to allow for exec to run. A lot of times this was done checking with pointer arithmetic.
Again, since print statements are not feasable, a lot of testing was also conceptual/drawn out to see if the control was actually behaving as intended, which could possibly mean iterating through a majority of the loop just to make it was correct.

After building out all the functions to presumably max efficieny, a bunch of test cases within the driver program were tested and measured to see if they worked correctly. Each test case was compared to its equivalent response within the regular bash terminal to see if they functioned as intended. If it didn't, it was hopefully intentional.



Documentation and commenting is done throughout all the files for general purpose and understanding.
