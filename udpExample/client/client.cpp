#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
  char msg1[] = "MESSAGE1";
  char msg2[] = "MESSAGE2";
  int sock;
  struct sockaddr_in addr;
  sock = socket(AF_INET, SOCK_DGRAM, 0);

  addr.sin_family = AF_INET;
  addr.sin_port = htons(3425);
  addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&addr, sizeof(addr));
  close(sock);
  return 0;
}
