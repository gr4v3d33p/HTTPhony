#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>

#define MAX_PORTS 1000

int main(int argc, char *argv[])
{
    int sockfd, n, num_ports = 0, i;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    const char *http_request = "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n";
    int ports[MAX_PORTS];

    if (argc < 3) {
        printf("Usage: %s hostname ports\n", argv[0]);
        exit(1);
    }

    // Parse port numbers from input
    char *ports_arg = argv[2];
    char *range = strstr(ports_arg, "-");
    if (range) {
        // Range of ports given
        int start_port = atoi(strtok(ports_arg, "-"));
        int end_port = atoi(strtok(NULL, "-"));
        for (i = start_port; i <= end_port && num_ports < MAX_PORTS; i++) {
            ports[num_ports++] = i;
        }
    } else {
        // List of ports given
        char *port_str = strtok(ports_arg, ",");
        while (port_str != NULL && num_ports < MAX_PORTS) {
            ports[num_ports++] = atoi(port_str);
            port_str = strtok(NULL, ",");
        }
    }

    // Scan each port in the list
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"Error, no such host\n");
        exit(1);
    }

    for (i = 0; i < num_ports; i++) {
        int portno = ports[i];
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Error opening socket");
            exit(1);
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(portno);

        if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
            printf("Port %d is closed\n", portno);
        } else {
            printf("Port %d is open\n", portno);
            // Send HTTP request to check if port is open
            sprintf(buffer, http_request, argv[1]);
            n = write(sockfd, buffer, strlen(buffer));
            // Ignore response from server
            bzero(buffer, sizeof(buffer));
            n = read(sockfd, buffer, sizeof(buffer) - 1);
        }
        close(sockfd);
        
        // Add delay between port scans
        if (argc > 3) {
            int delay = atoi(argv[3]);
            sleep(delay);
        }
    }

    exit(0);
}
