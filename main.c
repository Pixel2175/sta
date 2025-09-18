#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#define SOCKET_PATH "/tmp/sta"

typedef struct {
    int id;
    char *name;
} Pro;

typedef struct {
    Pro *process;
    int count;
} Pros;

void error(char *t,char *m,...){
    va_list args;
    va_start(args,m);
    fprintf(stderr, "\033[1;31m[E]\033[0m \033[1;33m%s\033[0m:", t);
    vfprintf(stderr, m,args);
    fprintf(stderr, "\n");
    va_end(args);
}

Pros *init(){
    Pros *list = malloc(sizeof(Pros));
    list->process = malloc(sizeof(Pro));
    list->count = 0;
    return list;
}

void add_pro(Pros *list,int id,char *name){
	int checker = 0;
	for (int i = 0;i<list->count;++i){
		if (id == list->process[i].id){
			list->process[i].name = strdup(name);
			checker = 1;
		}
	}
	if (checker) return;

    list->count++;
    list->process = realloc(list->process, sizeof(Pro) * list->count);
    if (!list->process) error("Memory","memory allocation failed.");
    list->process[list->count -1].name = strdup(name);
    list->process[list->count -1].id = id;
}

void show_pros(Pros *list)
{
    for(int a = 0; a<list->count;++a){
        printf("%s",list->process[a].name);
    }
	printf("\n");
    fflush(stdout);
}

void show_help()
{
	printf("Usage:\n");
	printf("\t-id        <number>       => process id (id >= 1)\n");
	printf("\t-name      <string>       => process name\n");
	printf("\t-h -help                  => print this help menu\n");

	exit(1);
}

void args(int *id,const char **name,int argc, char const* argv[])
{
	for (int i = 1; i < argc; i += 2) {
		if (!strcmp("-h", argv[i]) ||!strcmp("-help", argv[i])) 
			show_help();

		if (i + 1 >= argc || argv[i+1][0] == '-') 
			error("Args", "missing value for %s\nrun: sta -h", argv[i]);

		if (!strcmp("-id", argv[i])) 
			*id = atoi(argv[i + 1]);

		else if (!strcmp("-name", argv[i])) 
			*name = argv[i + 1];

	}

	if (*id <= 0 || !*name) 
		show_help();

}

void clean_up(){
    unlink(SOCKET_PATH);
    exit(0);
}

int main(int argc, char const* argv[]){

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, SOCKET_PATH);
    int sock = socket(AF_UNIX, SOCK_STREAM , 0);
    if (sock < 0) error("Socket","failed to create socket: %s", strerror(errno));

    Pros *list = init();

    if (access(SOCKET_PATH, F_OK) == 0) {
		int id;
		const char *name;
		char buf[256];
		args(&id, &name,argc, argv);
		snprintf(buf,sizeof(buf),"%d|+|%s",id,name);

        if (connect(sock, (struct sockaddr*)&addr , sizeof(addr)) < 0)
            error("Connect","failed: %s", strerror(errno));

        write(sock, buf, strlen(buf));
        close(sock);
        return 0;
    } else {
        signal(SIGTERM, clean_up);
        signal(SIGINT, clean_up);

        unlink(SOCKET_PATH);
        if (bind(sock, (struct sockaddr*)&addr , sizeof(addr)) < 0)
            error("Bind","failed: %s", strerror(errno));

        if (listen(sock, 5) < 0)
            error("Listen","failed: %s", strerror(errno));

        while (1) {
            int client = accept(sock, NULL, NULL);
            if (client < 0) continue;

            char buf[256], name[256];
            int n,id;

            while ((n = recv(client, buf, sizeof(buf)-1, 0)) > 0) {
                buf[n] = '\0';
				char *s = strstr(buf,"|+|");
				if (s){
					*s = '\0';
					id = atoi(buf);
					strncpy(name,s+3,sizeof(name)-1);
					add_pro(list,  id, name);
					show_pros(list);
				}
            }
            close(client);
        }
        close(sock);
        unlink(SOCKET_PATH);
        return 0;
    }
}

