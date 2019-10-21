/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Name: calculator_client_main
 ****************************************************************************/

#include "../calculator_server/CalculatorServiceClientStub.h"
#include "../calculator_server/Channel.h"

#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define TEST_N (100000)

extern "C" {

void *thread(void *arg)
{
	for (int i = 0; i < TEST_N; i++) {
		pthread_yield();
	}

	return NULL;
}

int calculator_client_main(int argc, char *argv[])
{
	printf("calculator_client start\n");
	CalculatorServiceClientStub service;

	service.print("print test\n");
	printf("1 + 2 = %d\n", service.add(1, 2));
	printf("square(4) = %d\n", service.square(4));

	struct timespec start, end;
	{
		clock_gettime(CLOCK_REALTIME, &start);

		for (int i = 0; i < TEST_N; i++) {
			int sum = service.add(i, i + 1);
			if (sum != i + i + 1) {
				printf("%d + %d != %d service add failed\n", i, i + 1, sum);
			}
		}

		clock_gettime(CLOCK_REALTIME, &end);
		printf("%d times rpc takes %lld(ms)\n", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec)) / 1000000);
	}

	{
		clock_gettime(CLOCK_REALTIME, &start);

		for (int i = 0; i < TEST_N; i++) {
			int sum = i + i + 1;
			if (i == TEST_N / 2) {
				printf("%d + %d == %d\n", i, i + 1, sum);
			}
		}

		clock_gettime(CLOCK_REALTIME, &end);
		printf("%d times add takes %lld(ms)\n", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec)) / 1000000);
	}

	{
		clock_gettime(CLOCK_REALTIME, &start);

		for (int i = 0; i < TEST_N; i++) {
			Channel("/calculator/test");
		}

		clock_gettime(CLOCK_REALTIME, &end);
		printf("%d times Channel open/close takes %lld(ms)\n", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec)) / 1000000);
	}

	{
		char buf[128];

		clock_gettime(CLOCK_REALTIME, &start);

		auto ch = Channel("/calculator/test");
		for (int i = 0; i < TEST_N; i++) {
			ch.send("1234", 5);
			ch.receive(buf, 128);
		}

		clock_gettime(CLOCK_REALTIME, &end);
		printf("%d times Channel send/receive takes %lld(ms)\n", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec)) / 1000000);
	}

	{
		pthread_t pid;
		pthread_create(&pid, NULL, thread, NULL);

		clock_gettime(CLOCK_REALTIME, &start);

		for (int i = 0; i < TEST_N; i++) {
			pthread_yield();
		}

		clock_gettime(CLOCK_REALTIME, &end);

		pthread_join(pid, NULL);
		printf("%d times task switching %lld(ms)\n", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec)) / 1000000);
	}
	return 0;
}
}
