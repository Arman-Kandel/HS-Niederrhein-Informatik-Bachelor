scp -F ./ssh-config ./main.c bsy-pi:~/project/main.c
ssh -F ./ssh-config bsy-pi
gcc ./project/main.c -o ./project/exe -lpthread
./project/exe
pause