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
 * Name: helloxx_client_main
 ****************************************************************************/

#include "../helloxx_server/HelloServiceClientStub.h"

#include <stdio.h>
#include <time.h>

#define TEST_N (10)

extern "C"
{
int helloxx_client_main(int argc, char *argv[])
{
	printf("helloxx_client start\n");
	HelloServiceClientStub service;
	
	struct timespec start, end;
	clock_gettime(CLOCK_REALTIME, &start);

	for (int i = 0; i < TEST_N; i++)
	{
		int sum = service.add(i, i + 1);
		if (sum != i + i + 1)
		{
			printf("service add failed\n");
		}
	}

	clock_gettime(CLOCK_REALTIME, &end);

	printf("%d times encode/decode takes %lld(ms)", TEST_N, ((end.tv_sec - start.tv_sec) * 1000000000LL + (end.tv_nsec - start.tv_nsec))/ 1000000);
	return 0;
}
}
