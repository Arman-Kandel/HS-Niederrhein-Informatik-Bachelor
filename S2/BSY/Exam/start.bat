scp -F ./ssh-config ./main.c bsy-pi:~/project/main.c
scp -F ./ssh-config ./my-utils.h bsy-pi:~/project/my-utils.h
scp -F ./ssh-config ./build-script.sh bsy-pi:~/project/build-script.sh
ssh -F ./ssh-config -t bsy-pi "chmod 777 ./project/build-script.sh | ./project/build-script.sh; bash -l"
pause