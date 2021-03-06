#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
int s_socket, s_server;
struct lookup
{
	char s;
	char code[200];
};

struct lookup LookUpTable[200];

int ServerCreate(int port)		
{
	s_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;

	
	if(bind(s_socket, (struct sockaddr*)&server, sizeof(server)) != -1)
	{
		printf("[+]Server Running.\n");
		listen(s_socket, 5);
		socklen_t add;
		add = sizeof(other);
		s_server = accept(s_socket, (struct sockaddr*)&other, &add);
		return 0;
	}
	else
	{
		return 1; //Error
	} 
}
void Decoder(char seq[], char chars[], int n){

	char code[200];
	int code_size = 0;

	printf("Decoded sequence: ");
	for(int i=0;i<n;i++)
	{
		code[code_size] = seq[i];
		code_size++;

		char s;

		int flag = 0;
		for(int j=0;j<strlen(chars);j++)
		{
			if(strlen(LookUpTable[(int)chars[j]].code) == code_size)
			{
				int flag2 = 1;
				for(int k=0;k<code_size;k++)
				{
					if(LookUpTable[(int)chars[j]].code[k] != code[k]) flag2 = 0;
				}
				if(flag2 == 1)
				{
					flag = 1;
					s = chars[j];
				}
			}
		}
		if(flag == 1)
		{
			printf("%c", s);
			code_size = 0;
		}
	}
	printf("\n");
}

int main()
{
	printf("\n");
	int error = ServerCreate(9009);
	if(error == 1)
	{
		close(s_server);
		close(s_socket);
		printf("Server Issues.\n");
		return 0;
	}
	else 
	{
		printf("\nServer Started\n");

		char encodedseq[200];
		char chars[200];

		char s[1];
		char code[100];

		recv(s_server, chars, sizeof(chars), 0);
		
		for(int i=0;i<strlen(chars);i++)
		{
			recv(s_server, s, sizeof(s), 0);
			recv(s_server, code, sizeof(code), 0);

			LookUpTable[(int)chars[i]].s = s[0];
			strcpy(LookUpTable[(int)chars[i]].code, code);
		}

		recv(s_server, encodedseq, sizeof(encodedseq), 0);

		printf("\nCharacters: %s\n\n", chars);
		for(int i=0;i<strlen(chars);i++)
		{
			printf("%c - %s\n", LookUpTable[(int)chars[i]].s, LookUpTable[(int)chars[i]].code);
		}

		Decoder(encodedseq, chars, strlen(encodedseq));
		
		printf("Completed.\n");
	}

	close(s_server);
	close(s_socket);

	printf("\n");	
	return 0;
}
