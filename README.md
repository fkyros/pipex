# pipex

## how to clone:
git clone <url>
git submodule init
git submodule update

or git clone --recurse-submodules <url>

## execution:
./pipex file1 "cmd1" "cmd2" file2

works as this command shell: 
$> < file1 cmd1 | cmd2 > file2

## idea
father process will have two child, each one will execute the first and the second command correspondingly.
the father then will wait those interconnected process by a pipe to finish, and then exit the program accordingly.

### how a pipe works
res = pipe(fd[2])

when a pipe is opened, the resulting file descriptors open are fd[0] and fd[1]:
- fd[0] where you read from
- fd[1] where you write into

### how dup2 works
int dup2(int oldfd, int newfd)

oldfd is where you will be taking information, and the newfd is what fd you are substituting

e.g.
fd = open("file", O\_RDONLY); -> file to read from as if it was STDIN
dup2(fd, STDIN\_FILENO); -> replace stdin with the new file descriptor
close(fd); -> close the old and duplicated file descriptor
exec(...); -> now the program executed on this new process will read from the file instead of the user input from the keyboard as usual

# classic executions for testing

- ./pipex /dev/fd/0 cat ls /dev/fd/1
-> checks if 'wait()' functions are done in the father process
-> should let you input on stdin and then close

- ./pipex Makefile "" ls /dev/fd/1
-> checks if the null command doesn't segfault the program, and then outputs through stderr the invalid command (as in bash)
-> should input the ls 
-> $? = 0

- ./pipex Makefile ls clkdsj outfile
-> print on stderr that the command do not exist
-> create the outfile (no matter if the second command failed)
-> error code ($?) must be 127

- ./pipex here\_doc LIM cat ls outfile
-> make sure the outfile is APPENDED and not TRUNCATED
