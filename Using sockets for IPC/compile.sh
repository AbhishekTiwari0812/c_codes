gcc network.c -o network graph.c -w
gcc end_node.c -o end_node graph.c  socket_open_read_write.c message_encode_decode.c -w
gcc router.c -o router graph.c socket_open_read_write.c -w message_encode_decode.c -w
gcc test_client.c -o test_client message_encode_decode.c socket_open_read_write.c graph.c -w

