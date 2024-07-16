/* SPDX-License-Identifier: GPL-3.0-only */
/*
 * Test program against ioctl for Char Device LKM
 *
 * Author: Pairman Guo <pairmanxlr@gmail.com>
 *
 * Copyright (C) 2024 Pairman Guo <pairmanxlr@gmail.com>
 */

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

#include "module.h"
#include "fops.h"

#define KBUILD_MODNAME "chardev"
#define DEVICE_FNAME "/dev/"DEVICE_NAME

/**
 * test_ioctl_set - test ioctl set
 *
 * @fd: the file descriptor
 * @buf: pointer of the buffer to set
 *
 * Test charp type ioctl _IOW (IOCTL_DBUF_SET).
 */
void test_ioctl_set(int fd, char *buf)
{
	int ret;
	ret = ioctl(fd, IOCTL_DBUF_SET, buf);
	if (ret < 0) {
		printf("[%s] (%s) failed with %d.\n", KBUILD_MODNAME, __func__,
		       ret);
		exit(-1);
	}
	printf("[%s] (%s) set to \"%s\".\n", KBUILD_MODNAME, __func__, buf);
}

/**
 * test_ioctl_get - test ioctl get
 *
 * @fd: the file descriptor
 *
 * Test charp type ioctl _IOR (IOCTL_DBUF_GET).
 */
void test_ioctl_get(int fd)
{
	char buf[BUF_LEN];
	int ret;
	ret = ioctl(fd, IOCTL_DBUF_GET, buf);
	if (ret < 0) {
		printf("[%s] (%s) failed with %d.\n", KBUILD_MODNAME, __func__,
		       ret);
		exit(-1);
	}
	printf("[%s] (%s) got \"%s\".\n", KBUILD_MODNAME, __func__, buf);
}

/**
 * test_ioctl_setn - test ioctl setn
 *
 * @fd: the file descriptor
 *
 * Test int type ioctl _IOW (IOCTL_DBUF_SETN).
 */
void test_ioctl_setn(int fd, int i)
{
	int ret;
	int j = 0;
	for (; j < 6; ++j) {
		ret = ioctl(fd, IOCTL_DBUF_SETN, ++i);
		if (ret < 0) {
			printf("[%s] (%s) failed with %d at index %d.\n",
			       KBUILD_MODNAME, __func__, ret, i);
			exit(-1);
		}
		printf("[%s] (%s) set successfully at index %d.\n",
		       KBUILD_MODNAME, __func__, i);
	}
}

/**
 * test_ioctl_getn - test ioctl getn
 *
 * @fd: the file descriptor
 *
 * Test int type ioctl _IOR (IOCTL_DBUF_GETN).
 */
void test_ioctl_getn(int fd)
{
	char buf[BUF_LEN];
	int ret = '0';
	int i = 0;
	for (i; ret; ++i) {
		ret = ioctl(fd, IOCTL_DBUF_GETN, i);
		if (ret < 0) {
			printf("[%s] (%s) failed with %d at index %d.\n",
			       KBUILD_MODNAME, __func__, ret, i);
			exit(-1);
		}
		buf[i] = (char)ret;
	}
	printf("[%s] (%s) got \"%s\"\n", KBUILD_MODNAME, __func__, buf);
}

/**
 * main - run tests
 *
 * Run all the ioctl tests.
 */
int main()
{
	char *buf = " b i g   y i k e s . ";
	int blen = strlen(buf);
	int fd;
	fd = open(DEVICE_FNAME, 0);
	if (fd < 0) {
		printf("[%s] (%s) cannot open device file %s.\n",
		       KBUILD_MODNAME, __func__, DEVICE_FNAME);
		exit(-1);
	}
	test_ioctl_get(fd);
	test_ioctl_set(fd, buf);
	srand(time(NULL));
	test_ioctl_setn(fd, rand() % (blen - 6));
	test_ioctl_getn(fd);
	close(fd);
	return 0;
}
