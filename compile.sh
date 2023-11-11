built_cmd=''
function build_cmd(){
    # function to return gcc command with all arguments
    files="$(ls -1 . | grep -v 'main.c' | grep -v 'compile.sh' | grep '.c' | tr '\n' ' ')"
    built_cmd="gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -g main.c $files -o hsh"
}
echo Starting compilation
build_cmd
$built_cmd && echo Finished compilation successfuly && echo Starting shell && ./hsh
