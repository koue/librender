/*
 * Copyright (c) 2020-2022 Nikola Kolev <koue@chaosophia.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "render.h"

typedef struct {
	char *p1;
	char *p2;
	int pi;
} mydata;

static struct render render;

static
void render_middle(const char *macro, void *arg)
{
	mydata *item = (mydata *)arg;

	assert(macro != NULL);
	assert(item == NULL);
	assert(strcmp(macro, "MIDDLE") == 0);
}

static
void render_top(const char *macro, void *arg)
{
	mydata *item = (mydata *)arg;

	assert(macro != NULL);
	assert(item == NULL);
	assert(strcmp(macro, "TOP") == 0);
}

static
void render_simple(const char *macro, void *arg)
{
	mydata *item = (mydata *)arg;

	assert(macro != NULL);
	assert(item != NULL);
	assert(strcmp(macro, "MACRO_SIMPLE") == 0);
	assert(strcmp(item->p1, "param1") == 0);
	assert(strcmp(item->p2, "param2") == 0);
	assert(item->pi == 10);
}

static
void render_data(const char *macro, void *arg)
{
	mydata *item = (mydata *)arg;

	assert(macro != NULL);
	assert(item != NULL);
	assert((strcmp(macro, "TOP") == 0) ||
		(strcmp(macro, "MIDDLE") == 0) ||
		(strcmp(macro, "BOTTOM") == 0));
	assert(strcmp(item->p1, "param1") == 0);
	assert(strcmp(item->p2, "param2") == 0);
	assert(item->pi == 10);
	if (strcmp(macro, "TOP") == 0) {
		assert(render_run(&render, "TOP", NULL) == 0);
	}
	if (strcmp(macro, "MIDDLE") == 0) {
		assert(render_run(&render, "MIDDLE", NULL) == 0);
	}
}

int
main(void)
{
	struct render_entry *entry;
	mydata DATA;

	DATA.p1 = "param1";
	DATA.p2 = "param2";
	DATA.pi = 10;

	/* init */
	render_init(&render);
	/* NO MACRO */
	assert(render_get(&render, "NOMACRO") == NULL);
	assert(render_add(&render, NULL, NULL, (mydata *)render_data) == -1);
	/* TOP */
	assert(render_get(&render, "TOP") == NULL);
	assert(render_add(&render, "TOP", NULL, (mydata *)render_top) == 0);
	assert((entry = render_get(&render, "TOP")) != NULL);
	/* MIDDLE */
	assert(render_get(&render, "MIDDLE") == NULL);
	assert(render_add(&render, "MIDDLE", "RENDER_MIDDLE.template",
	    (mydata *)render_middle) == 0);
	assert((entry = render_get(&render, "MIDDLE")) != NULL);
	/* MACRO_FILE */
	assert(render_get(&render, "MACRO_FILE") == NULL);
	assert(render_add(&render, "MACRO_FILE", "RENDER.template",
	    (mydata *)render_data) == 0);
	assert((entry = render_get(&render, "MACRO_FILE")) != NULL);
	assert(render_run(&render, "MACRO_FILE", (void *)&DATA) == 0);
	assert(render_remove(&render, "MACRO_FILE") == 0);
	assert(render_remove(&render, "notexist") == -1);
	assert(render_get(&render, "MACRO_FILE") == NULL);
	/* MACRO_SIMPLE */
	assert(render_get(&render, "MACRO_SIMPLE") == NULL);
	assert(render_add(&render, "MACRO_SIMPLE", NULL,
	    (mydata *)render_simple) == 0);
	assert((entry = render_get(&render, "MACRO_SIMPLE")) != NULL);
	assert(render_run(&render, "MACRO_SIMPLE", (void *)&DATA) == 0);
	assert(render_remove(&render, "MACRO_SIMPLE") == 0);
	assert(render_remove(&render, "notexist") == -1);
	assert(render_get(&render, "MACRO_SIMPLE") == NULL);
	/* purge */
	render_purge(&render);

	return (0);
}
