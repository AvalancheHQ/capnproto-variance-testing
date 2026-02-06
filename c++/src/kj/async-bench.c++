// Copyright (c) 2025 Cloudflare, Inc. and contributors
// Licensed under the MIT License:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Micro-benchmarks for KJ async stack.

#include <benchmark/benchmark.h>

#include <kj/async.h>
#include <kj/debug.h>

#include "callgrind.h"

// kj::READY_NOW is in its own performance class


// // https://codspeed.io/AvalancheHQ/capnproto-variance-testing/runs/compare/6981d46a6ba6a1b99e6888c1..6981d5cb4b57099e55a7908d?q=bm_Promise_ReadyNow
// // 5.5us -> 5.7us
// static void bm_Promise_ReadyNow(benchmark::State &state) {
//   // Benchmark waiting for a kj::READY_NOW promise.
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0); 

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = []() -> kj::Promise<void> { return kj::READY_NOW; }();
//       promise.wait(waitScope);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = []() -> kj::Promise<void> { return kj::READY_NOW; }();
//     promise.wait(waitScope);
//   }
// }

// BENCHMARK(bm_Promise_ReadyNow)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// ///////////////////////////////
// // Benchmarks for immediate promises and coroutines.

// kj::Promise<size_t> immediatePromise() { return 42; }

// static void bm_Promise_Immediate(benchmark::State &state) {
//   // Benchmark waiting for an immediate promise.
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = immediatePromise();
//       promise.wait(waitScope);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = immediatePromise();
//     promise.wait(waitScope);
//   }
// }

// BENCHMARK(bm_Promise_Immediate)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// kj::Promise<size_t> immediateCoroutine() { co_return 42; }

// static void bm_Coro_Immediate(benchmark::State &state) {
//   // Benchmark waiting for an immediate coroutine.
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = immediateCoroutine();
//       promise.wait(waitScope);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = immediateCoroutine();
//     promise.wait(waitScope);
//   }
// }

// BENCHMARK(bm_Coro_Immediate)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// ///////////////////////////////
// // Benchmarks for awaiting single immediate promises and coroutines.

// static void bm_Promise_ImmediatePromise_Then(benchmark::State &state) {
//   // Benchmark coro that co_awaits an immediate coroutine
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = immediatePromise().then([](size_t x) { return; });
//       promise.wait(waitScope);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = immediatePromise().then([](size_t x) { return; });
//     promise.wait(waitScope);
//   }
// }

// BENCHMARK(bm_Promise_ImmediatePromise_Then)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// static void bm_Coro_CoAwait_ImmediatePromise(benchmark::State &state) {
//   // Benchmark coro that co_awaits an immediate coroutine
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = []() -> kj::Promise<void> { co_await immediatePromise(); }();
//       promise.wait(waitScope);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = []() -> kj::Promise<void> { co_await immediatePromise(); }();
//     promise.wait(waitScope);
//   }
// }

// BENCHMARK(bm_Coro_CoAwait_ImmediatePromise)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// static void bm_Coro_CoAwait_ImmediateCoroutine(benchmark::State &state) {
//   // Benchmark coro that co_awaits an immediate coroutine
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = []() -> kj::Promise<void> {
//         co_await immediateCoroutine();
//       }();
//       promise.wait(waitScope);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = []() -> kj::Promise<void> {
//       co_await immediateCoroutine();
//     }();
//     promise.wait(waitScope);
//   }
// }

// BENCHMARK(bm_Coro_CoAwait_ImmediateCoroutine)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// ///////////////////////////////
// // Pow benchmarks mean to benchmark promise evaluation when the start of the
// // chain is immediate value.

// // pow2(i) = 2^i by successive doubling of 1.
// kj::Promise<size_t> pow2(size_t i) {
//   if (i == 0)
//     return 1;
//   return pow2(i - 1).then([](size_t x) { return x << 1; });
// }

// static void bm_Promise_Pow2_20(benchmark::State &state) {
//   // Benchmark waiting for an immediate promise.
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = pow2(20);
//       KJ_REQUIRE(promise.wait(waitScope) == 1ll << 20);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = pow2(20);
//     KJ_REQUIRE(promise.wait(waitScope) == 1ll << 20);
//   }
// }

// BENCHMARK(bm_Promise_Pow2_20)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// kj::Promise<size_t> coroPow2(size_t i) {
//   if (i == 0)
//     co_return 1;
//   co_return (co_await coroPow2(i - 1)) << 1;
// }

// static void bm_Coro_Pow2_20(benchmark::State &state) {
//   // Benchmark waiting for an immediate promise.
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto promise = coroPow2(20);
//       KJ_REQUIRE(promise.wait(waitScope) == 1ll << 20);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto promise = coroPow2(20);
//     KJ_REQUIRE(promise.wait(waitScope) == 1ll << 20);
//   }
// }

// BENCHMARK(bm_Coro_Pow2_20)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

///////////////////////////////
// shift benchmarks mean to benchmark deep promise chains ending on paf.

// shifts x left by n bits.
kj::Promise<size_t> shift(size_t n, kj::Promise<size_t> x) {
  if (n == 0)
    return x;
  return shift(n - 1, kj::mv(x)).then([](size_t x) { return x << 1; });
}

// benchmarks shift with unresolved paf and its fulfillment
static void bm_Promise_Shift_20(benchmark::State &state) {
  kj::EventLoop loop;
  kj::WaitScope waitScope(loop);

  // auto warmup_iters = state.range(0);
  for (auto _ : state) {
    // for (int64_t i = 0; i < warmup_iters; i++) {
    //   auto paf = kj::newPromiseAndFulfiller<size_t>();
    //   auto promise = shift(20, kj::mv(paf.promise));
    //   paf.fulfiller->fulfill(3);
    //   KJ_REQUIRE(promise.wait(waitScope) == (1ll << 20) * 3);
    // }
    // CALLGRIND_ZERO_STATS;

    auto paf = kj::newPromiseAndFulfiller<size_t>();
    auto promise = shift(20, kj::mv(paf.promise));
    paf.fulfiller->fulfill(3);
    KJ_REQUIRE(promise.wait(waitScope) == (1ll << 20) * 3);
  }
}

BENCHMARK(bm_Promise_Shift_20)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// // shifts x left by n bits.
// kj::Promise<size_t> coroShift(size_t n, kj::Promise<size_t> x) {
//   if (n == 0)
//     co_return co_await x;
//   co_return (co_await coroShift(n - 1, kj::mv(x))) << 1;
// }

// // benchmarks coroutine shift with unresolved paf and its fulfillment
// static void bm_Coro_Shift_20(benchmark::State &state) {
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   auto warmup_iters = state.range(0);

//   for (auto _ : state) {
//     for (int64_t i = 0; i < warmup_iters; i++) {
//       auto paf = kj::newPromiseAndFulfiller<size_t>();
//       auto promise = coroShift(20, kj::mv(paf.promise));
//       paf.fulfiller->fulfill(3);
//       KJ_REQUIRE(promise.wait(waitScope) == (1ll << 20) * 3);
//     }

//     CALLGRIND_ZERO_STATS;

//     auto paf = kj::newPromiseAndFulfiller<size_t>();
//     auto promise = coroShift(20, kj::mv(paf.promise));
//     paf.fulfiller->fulfill(3);
//     KJ_REQUIRE(promise.wait(waitScope) == (1ll << 20) * 3);
//   }
// }

// BENCHMARK(bm_Coro_Shift_20)->Arg(1)->Arg(5)->Arg(10)->Arg(20)->Arg(50)->Arg(100)->Arg(500)->Arg(1000);

// ///////////////////////////////
// // fib benchmarks mean to benchmark many await points within a single coro
// // these benchmark compute variant of fib function that sums previous 10 numbers.

// kj::Promise<size_t> promiseFib10(size_t i) {
//   if (i <= 10)
//     return 1;
//   return promiseFib10(i - 1).then([=](size_t x1) {
//     return promiseFib10(i - 2).then([=](size_t x2) {
//       return promiseFib10(i - 3).then([=](size_t x3) {
//         return promiseFib10(i - 4).then([=](size_t x4) {
//           return promiseFib10(i - 5).then([=](size_t x5) {
//             return promiseFib10(i - 6).then([=](size_t x6) {
//               return promiseFib10(i - 7).then([=](size_t x7) {
//                 return promiseFib10(i - 8).then([=](size_t x8) {
//                   return promiseFib10(i - 9).then([=](size_t x9) {
//                     return promiseFib10(i - 10).then([=](size_t x10) {
//                       return x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9 + x10;
//                     });
//                   });
//                 });
//               });
//             });
//           });
//         });
//       });
//     });
//   });
// }

// static void bm_Promise_Fib10(benchmark::State &state) {
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   for (auto _ : state) {
//     auto promise = promiseFib10(12);
//     KJ_REQUIRE(promise.wait(waitScope) == 19);
//   }
// }

// BENCHMARK(bm_Promise_Fib10);

// kj::Promise<size_t> coroFib10(size_t i) {
//   if (i <= 10)
//     co_return 1;
//   co_return (co_await coroFib10(i - 1)) + (co_await coroFib10(i - 2)) +
//       (co_await coroFib10(i - 3)) + (co_await coroFib10(i - 4)) +
//       (co_await coroFib10(i - 5)) + (co_await coroFib10(i - 6)) +
//       (co_await coroFib10(i - 7)) + (co_await coroFib10(i - 8)) +
//       (co_await coroFib10(i - 9)) + (co_await coroFib10(i - 10));
// }

// static void bm_Coro_Fib10(benchmark::State &state) {
//   kj::EventLoop loop;
//   kj::WaitScope waitScope(loop);

//   for (auto _ : state) {
//     auto promise = coroFib10(12);
//     KJ_REQUIRE(promise.wait(waitScope) == 19);
//   }
// }

// BENCHMARK(bm_Coro_Fib10);

///////////////////////////////
// Unused code for testing dead code elimination with LTO

// 1. Unused recursive fibonacci for testing dead code elimination
kj::Promise<size_t> unusedFibonacci(size_t n) {
  if (n <= 1) return n;
  return unusedFibonacci(n - 1).then([=](size_t a) {
    return unusedFibonacci(n - 2).then([=](size_t b) {
      return a + b;
    });
  });
}

// 2. Unused deep coroutine chain
kj::Promise<size_t> unusedDeepChain(size_t depth, size_t acc) {
  if (depth == 0) co_return acc;
  co_return co_await unusedDeepChain(depth - 1, acc * 2 + 1);
}

// 3. Unused promise tree builder
kj::Promise<void> unusedPromiseTree(size_t depth) {
  if (depth == 0) return kj::READY_NOW;
  auto left = unusedPromiseTree(depth - 1);
  auto right = unusedPromiseTree(depth - 1);
  return left.then([right = kj::mv(right)]() mutable {
    return kj::mv(right);
  });
}

// 4. Unused template-heavy code
template<size_t N>
kj::Promise<size_t> unusedTemplatedPower() {
  if constexpr (N == 0) {
    return 1;
  } else {
    return unusedTemplatedPower<N-1>().then([](size_t x) {
      return x * 2;
    });
  }
}

// Instantiate multiple template versions
kj::Promise<size_t> unusedPower5() { return unusedTemplatedPower<5>(); }
kj::Promise<size_t> unusedPower10() { return unusedTemplatedPower<10>(); }
kj::Promise<size_t> unusedPower15() { return unusedTemplatedPower<15>(); }
kj::Promise<size_t> unusedPower20() { return unusedTemplatedPower<20>(); }

// 5. Unused large static data structure
static constexpr size_t UNUSED_LOOKUP_TABLE[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
  20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
  40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
  60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,
  80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99,
  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
  120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
  140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
  160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
  180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
  200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
  220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
  240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259,
  260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279,
  280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
  300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319,
  320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339,
  340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359,
  360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379,
  380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399,
  400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419,
  420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439,
  440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459,
  460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479,
  480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499,
  500, 501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519,
  520, 521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539,
  540, 541, 542, 543, 544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559,
  560, 561, 562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579,
  580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597, 598, 599,
  600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619,
  620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639,
  640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659,
  660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679,
  680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699,
  700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719,
  720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739,
  740, 741, 742, 743, 744, 745, 746, 747, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759,
  760, 761, 762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779,
  780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799,
  800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819,
  820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839,
  840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859,
  860, 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879,
  880, 881, 882, 883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899,
  900, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914, 915, 916, 917, 918, 919,
  920, 921, 922, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934, 935, 936, 937, 938, 939,
  940, 941, 942, 943, 944, 945, 946, 947, 948, 949, 950, 951, 952, 953, 954, 955, 956, 957, 958, 959,
  960, 961, 962, 963, 964, 965, 966, 967, 968, 969, 970, 971, 972, 973, 974, 975, 976, 977, 978, 979,
  980, 981, 982, 983, 984, 985, 986, 987, 988, 989, 990, 991, 992, 993, 994, 995, 996, 997, 998, 999
};

size_t unusedLookup(size_t idx) {
  return UNUSED_LOOKUP_TABLE[idx % 1000];
}

BENCHMARK_MAIN();
