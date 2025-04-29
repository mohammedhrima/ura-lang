struct in_addr:
   int s_addr

struct sockaddr_in:
   short sin_family
   short sin_port
   in_addr sin_addr
   chars sin_zero

main():
   int server_fd = socket(2, 2, 0)
   if server_fd == 0:
      putstr("failed to create socket")
      exit(1)
   
   #int opt = 1
   // takes ref to opt
   #if setsockopt(server_fd, 1, 2, opt, 4) < 0:
   #   putstr("failed to setsockopt")
   #   exit(1)
   
   sockaddr_in address
   address.sin_zero = malloc(8)
   address.sin_family = 2
   address.sin_addr.s_addr = 0
   address.sin_port = htons(17000)

   if bind(server_fd, address, sizeof(address)) < 0:
      putstr("bind failed")
      exit(1)
   
   if listen(server_fd, 3) < 0:
      putstr("listen")
      exit(1)
   
   putstr("server listening op port 17000")


   