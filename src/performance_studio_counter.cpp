/**
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2023-2024, Arm Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "performance_studio_counter.h"
#include <godot_cpp/classes/mutex.hpp>

#if defined(__ANDROID__)
#include "streamline_annotate.h"
#endif

using namespace godot;

uint32_t PerformanceStudio_Counter::count = 0;
static Ref<Mutex> countMutex;

void PerformanceStudio_Counter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_value", "value"), &PerformanceStudio_Counter::set_value);
	countMutex.instantiate();
}

void PerformanceStudio_Counter::cleanup() {
	countMutex.unref();
}

void PerformanceStudio_Counter::set_value(float value) {
#if defined(__ANDROID__)
	uint32_t ivalue = (uint32_t)(value * (float)modifier);
	gator_annotate_counter_value(0, id, ivalue);
#endif
}

void PerformanceStudio_Counter::init(String title, String series, bool delta) {
#if defined(__ANDROID__)
	gator_annotate_counter_class counter_class = delta ? ANNOTATE_DELTA : ANNOTATE_ABSOLUTE;
	gator_annotate_display display_class = delta ? ANNOTATE_ACCUMULATE : ANNOTATE_MAXIMUM;

	gator_annotate_counter(
		id, title.ascii().get_data(), series.ascii().get_data(), 0, counter_class, display_class,
		NULL, modifier, ANNOTATE_OVERLAY, ANNOTATE_LINE, 0, 0, 0,
		0, NULL, NULL, 0, 0, NULL);
#endif
}

PerformanceStudio_Counter::PerformanceStudio_Counter() {
	countMutex->lock();
	id = ++count;
	countMutex->unlock();
}
