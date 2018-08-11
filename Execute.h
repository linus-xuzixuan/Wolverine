#include <cstdlib>
#include <string>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 300;

void Execute(string cmnd){
    char line[N];
    FILE *fp;
    string cmd = cmnd;
    const char *sysCommand = cmd.data();
    if ((fp = popen(sysCommand, "r")) == NULL) {
        cout << "error" << endl;
        return;
    }
    while (fgets(line, sizeof(line)-1, fp) != NULL){
        cout << line ;
    }
    pclose(fp);
}
