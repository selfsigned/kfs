.global syscall_stub /* export our stub */
.extern syscall_handler

syscall_stub:
  pushal
  call syscall_handler
  popal
  iret
