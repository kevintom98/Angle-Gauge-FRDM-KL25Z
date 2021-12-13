#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "cbfifo.h"


/*
 *This function tests the cbfifo.c through different cases.
 *Asserts are used for testing. Program stops is there is a problem.
 *
 *Parameters:
 *	none
 *
 *return:
 *	none
 */
void test_cbfifo()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf[1024];
  const int cap = cbfifo_capacity(RX_Buffer);



  // asserts in following 2 lines -- this is not TX_Buffering the student,
  // it's validating that the TX_Buffer is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  assert(cap == 256 || cap == 127);


  assert(cbfifo_length(RX_Buffer) == 0);
  assert(cbfifo_dequeue(buf, cap,RX_Buffer) == 0);
  assert(cbfifo_dequeue(buf, 1,RX_Buffer) == 0);

  assert(cbfifo_length(TX_Buffer) == 0);
  assert(cbfifo_dequeue(buf, cap,TX_Buffer) == 0);
  assert(cbfifo_dequeue(buf, 1,TX_Buffer) == 0);

  // enqueue 10 bytes, then dequeue same amt
  assert(cbfifo_enqueue(str, 10,RX_Buffer) == 10);
  assert(cbfifo_length(RX_Buffer) == 10);
  assert(cbfifo_dequeue(buf, 10,RX_Buffer) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(cbfifo_length(RX_Buffer) == 0);



   // enqueue 10 bytes, then dequeue same amt
  assert(cbfifo_enqueue(str, 10,TX_Buffer) == 10);
  assert(cbfifo_length(TX_Buffer) == 10);
  assert(cbfifo_dequeue(buf, 10,TX_Buffer) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(cbfifo_length(TX_Buffer) == 0);



  // enqueue 20 bytes;  dequeue 5, then another 20
  assert(cbfifo_enqueue(str, 20,RX_Buffer) == 20);
  assert(cbfifo_length(RX_Buffer) == 20);

   // enqueue 20 bytes;  dequeue 5, then another 20
  assert(cbfifo_enqueue(str, 20,TX_Buffer) == 20);
  assert(cbfifo_length(TX_Buffer) == 20);



  assert(cbfifo_dequeue(buf, 5, RX_Buffer) == 5);
  assert(cbfifo_length(RX_Buffer) == 15);
  assert(cbfifo_dequeue(buf+5, 20,RX_Buffer) == 15);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str, 20) == 0);

  assert(cbfifo_dequeue(buf, 5, TX_Buffer) == 5);
  assert(cbfifo_length(TX_Buffer) == 15);
  assert(cbfifo_dequeue(buf+5, 20,TX_Buffer) == 15);
  assert(cbfifo_length(TX_Buffer) == 0);
  assert(strncmp(buf, str, 20) == 0);

  // fill buffer and then read it back out
  assert(cbfifo_enqueue(str, cap, RX_Buffer) == cap);
  assert(cbfifo_length(RX_Buffer) == cap);
  assert(cbfifo_enqueue(str, 1,RX_Buffer) == 0);
  assert(cbfifo_dequeue(buf, cap,RX_Buffer) == cap);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // Add 20 bytes and pull out 18
  assert(cbfifo_enqueue(str, 20,RX_Buffer) == 20);
  assert(cbfifo_length(RX_Buffer) == 20);
  assert(cbfifo_dequeue(buf, 18,RX_Buffer) == 18);
  assert(cbfifo_length(RX_Buffer) == 2);
  assert(strncmp(buf, str, 18) == 0);


  // fill buffer and then read it back out
  assert(cbfifo_enqueue(str, cap, TX_Buffer) == cap);
  assert(cbfifo_length(TX_Buffer) == cap);
  assert(cbfifo_enqueue(str, 1,TX_Buffer) == 0);
  assert(cbfifo_dequeue(buf, cap,TX_Buffer) == cap);
  assert(cbfifo_length(TX_Buffer) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // Add 20 bytes and pull out 18
  assert(cbfifo_enqueue(str, 20,TX_Buffer) == 20);
  assert(cbfifo_length(TX_Buffer) == 20);
  assert(cbfifo_dequeue(buf, 18,TX_Buffer) == 18);
  assert(cbfifo_length(TX_Buffer) == 2);
  assert(strncmp(buf, str, 18) == 0);

  // Take out the 2 remaining bytes from above
  assert(cbfifo_dequeue(buf, 2, RX_Buffer) == 2);
  assert(strncmp(buf, str+18, 2) == 0);

  // Take out the 2 remaining bytes from above
  assert(cbfifo_dequeue(buf, 2, TX_Buffer) == 2);
  assert(strncmp(buf, str+18, 2) == 0);


  // write more than capacity
  assert(cbfifo_enqueue(str, 65, RX_Buffer) == 65);
  assert(cbfifo_enqueue(str+65, cap,RX_Buffer) == (cap-65));
  assert(cbfifo_length(RX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap,RX_Buffer) == cap);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // write more than capacity
  assert(cbfifo_enqueue(str, 65, TX_Buffer) == 65);
  assert(cbfifo_enqueue(str+65, cap,TX_Buffer) == (cap-65));
  assert(cbfifo_length(TX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap,TX_Buffer) == cap);
  assert(cbfifo_length(TX_Buffer) == 0);
  assert(strncmp(buf, str, cap) == 0);


  // write zero bytes
  assert(cbfifo_enqueue(str, 0, RX_Buffer) == 0);
  assert(cbfifo_length(RX_Buffer) == 0);

  // write zero bytes
  assert(cbfifo_enqueue(str, 0, TX_Buffer) == 0);
  assert(cbfifo_length(TX_Buffer) == 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  assert(cbfifo_enqueue(str, 32, RX_Buffer) == 32);  // advance so that read < write
  assert(cbfifo_length(RX_Buffer) == 32);
  assert(cbfifo_dequeue(buf, 16, RX_Buffer) == 16);
  assert(cbfifo_length(RX_Buffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str, 32, TX_Buffer) == 32);  // advance so that read < write
  assert(cbfifo_length(TX_Buffer) == 32);
  assert(cbfifo_dequeue(buf, 16, TX_Buffer) == 16);
  assert(cbfifo_length(TX_Buffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, 32, RX_Buffer) == 32);  // (1)
  assert(cbfifo_length(RX_Buffer) == 48);
  assert(cbfifo_enqueue(str+64, cap-64,RX_Buffer) == cap-64);  // (2)
  assert(cbfifo_length(RX_Buffer) == cap-16);
  assert(cbfifo_dequeue(buf+16, cap-16, RX_Buffer) == cap-16);
  assert(strncmp(buf, str, cap) == 0);

  assert(cbfifo_enqueue(str, 32, RX_Buffer) == 32);  // advance so that read < write
  assert(cbfifo_length(RX_Buffer) == 32);
  assert(cbfifo_dequeue(buf, 16, RX_Buffer) == 16);
  assert(cbfifo_length(RX_Buffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, 32, TX_Buffer) == 32);  // (1)
  assert(cbfifo_length(TX_Buffer) == 48);
  assert(cbfifo_enqueue(str+64, cap-64,TX_Buffer) == cap-64);  // (2)
  assert(cbfifo_length(TX_Buffer) == cap-16);
  assert(cbfifo_dequeue(buf+16, cap-16, TX_Buffer) == cap-16);
  assert(strncmp(buf, str, cap) == 0);

  assert(cbfifo_enqueue(str, 32, TX_Buffer) == 32);  // advance so that read < write
  assert(cbfifo_length(TX_Buffer) == 32);
  assert(cbfifo_dequeue(buf, 16, TX_Buffer) == 16);
  assert(cbfifo_length(TX_Buffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap-20, RX_Buffer) == cap-20);  // (3)
  assert(cbfifo_length(RX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, cap-8, RX_Buffer) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(cbfifo_length(RX_Buffer) == 4);
  assert(cbfifo_dequeue(buf, 8, RX_Buffer) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(cbfifo_length(RX_Buffer) == 0);

  assert(cbfifo_enqueue(str, 49 ,RX_Buffer) == 49);  // advance so that read < write
  assert(cbfifo_length(RX_Buffer) == 49);
  assert(cbfifo_dequeue(buf, 16, RX_Buffer) == 16);
  assert(cbfifo_length(RX_Buffer) == 33);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap-20, TX_Buffer) == cap-20);  // (3)
  assert(cbfifo_length(TX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, cap-8, TX_Buffer) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(cbfifo_length(TX_Buffer) == 4);
  assert(cbfifo_dequeue(buf, 8, TX_Buffer) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(cbfifo_length(TX_Buffer) == 0);

  assert(cbfifo_enqueue(str, 49 ,TX_Buffer) == 49);  // advance so that read < write
  assert(cbfifo_length(TX_Buffer) == 49);
  assert(cbfifo_dequeue(buf, 16, TX_Buffer) == 16);
  assert(cbfifo_length(TX_Buffer) == 33);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+49, cap-33, RX_Buffer) == cap-33);  // (4)
  assert(cbfifo_length(RX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap, RX_Buffer) == cap);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(str, 32, RX_Buffer) == 32);  // advance so that read < write
  assert(cbfifo_length(RX_Buffer) == 32);
  assert(cbfifo_dequeue(buf, 16, RX_Buffer) == 16);
  assert(cbfifo_length(RX_Buffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+49, cap-33, TX_Buffer) == cap-33);  // (4)
  assert(cbfifo_length(TX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap, TX_Buffer) == cap);
  assert(cbfifo_length(TX_Buffer) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(str, 32, TX_Buffer) == 32);  // advance so that read < write
  assert(cbfifo_length(TX_Buffer) == 32);
  assert(cbfifo_dequeue(buf, 16, TX_Buffer) == 16);
  assert(cbfifo_length(TX_Buffer) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(str+32, cap, RX_Buffer) == cap-16);  // (5)
  assert(cbfifo_dequeue(buf, 1, RX_Buffer) == 1);
  assert(cbfifo_length(RX_Buffer) == cap-1);
  assert(cbfifo_dequeue(buf+1, cap-1,RX_Buffer) == cap-1);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(str+32, cap, TX_Buffer) == cap-16);  // (5)
  assert(cbfifo_dequeue(buf, 1, TX_Buffer) == 1);
  assert(cbfifo_length(TX_Buffer) == cap-1);
  assert(cbfifo_dequeue(buf+1, cap-1,TX_Buffer) == cap-1);
  assert(cbfifo_length(TX_Buffer) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  assert(cbfifo_enqueue(str, cap-4, RX_Buffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(RX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32, RX_Buffer) == 32);
  rpos += 32;
  assert(cbfifo_length(RX_Buffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, RX_Buffer) == 12);
  wpos += 12;
  assert(cbfifo_length(RX_Buffer) == cap-24);


  assert(cbfifo_enqueue(str, cap-4, TX_Buffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(TX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32, TX_Buffer) == 32);
  rpos += 32;
  assert(cbfifo_length(TX_Buffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, TX_Buffer) == 12);
  wpos += 12;
  assert(cbfifo_length(TX_Buffer) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 16, RX_Buffer) == 16);  // (6)
  assert(cbfifo_length(RX_Buffer) == cap-8);
  assert(cbfifo_dequeue(buf, cap, RX_Buffer) == cap-8);
  assert(cbfifo_length(RX_Buffer) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, RX_Buffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(RX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32, RX_Buffer) == 32);
  rpos += 32;
  assert(cbfifo_length(RX_Buffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, RX_Buffer) == 12);
  wpos += 12;
  assert(cbfifo_length(RX_Buffer) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 16, TX_Buffer) == 16);  // (6)
  assert(cbfifo_length(TX_Buffer) == cap-8);
  assert(cbfifo_dequeue(buf, cap, TX_Buffer) == cap-8);
  assert(cbfifo_length(TX_Buffer) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, TX_Buffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(TX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32, TX_Buffer) == 32);
  rpos += 32;
  assert(cbfifo_length(TX_Buffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, TX_Buffer) == 12);
  wpos += 12;
  assert(cbfifo_length(TX_Buffer) == cap-24);


  assert(cbfifo_enqueue(str+wpos, 24, RX_Buffer) == 24);  // (7)
  assert(cbfifo_length(RX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap, RX_Buffer) == cap);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(str, cap-4, RX_Buffer) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(RX_Buffer) == cap-4);
  assert(cbfifo_dequeue(buf, 32, RX_Buffer) == 32);
  rpos += 32;
  assert(cbfifo_length(RX_Buffer) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(str+wpos, 12, RX_Buffer) == 12);
  wpos += 12;
  assert(cbfifo_length(RX_Buffer) == cap-24);

  assert(cbfifo_enqueue(str+wpos, 64, RX_Buffer) == 24);  // (8)
  assert(cbfifo_length(RX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap, RX_Buffer) == cap);
  assert(cbfifo_length(RX_Buffer) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  assert(cbfifo_enqueue(str+wpos, 64, TX_Buffer) == 24);  // (8)
  assert(cbfifo_length(TX_Buffer) == cap);
  assert(cbfifo_dequeue(buf, cap, TX_Buffer) == cap);
  assert(cbfifo_length(TX_Buffer) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  printf("\n\rCircular Buffer all test cases passed");
}
