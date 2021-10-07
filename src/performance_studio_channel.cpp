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

#include "performance_studio_channel.h"
#include "helpers.h"
#include <godot_cpp/classes/mutex.hpp>

#if defined(__ANDROID__)
#include "streamline_annotate.h"
#endif

using namespace godot;

uint32_t PerformanceStudio_Channel::count = 0;
static Ref<Mutex> countMutex;

void PerformanceStudio_Channel::_bind_methods() {
	ClassDB::bind_method(D_METHOD("annotate", "name"), &PerformanceStudio_Channel::annotate);
	ClassDB::bind_method(D_METHOD("annotate_color", "name", "color"), &PerformanceStudio_Channel::annotate_color);
	ClassDB::bind_method(D_METHOD("end"), &PerformanceStudio_Channel::end);
	countMutex.instantiate();
}

void PerformanceStudio_Channel::cleanup() {
	countMutex.unref();
}

void PerformanceStudio_Channel::annotate(String str) {
#if defined(__ANDROID__)
	gator_annotate_str(id, str.ascii().get_data());
#endif
}

void PerformanceStudio_Channel::annotate_color(String str, Color color) {
#if defined(__ANDROID__)
	uint32_t col = color_to_gator_int(color);
	gator_annotate_color(id, col, str.ascii().get_data());
#endif
}

void PerformanceStudio_Channel::end() {
#if defined(__ANDROID__)
	gator_annotate_str(id, NULL);
#endif
}

void PerformanceStudio_Channel::init(String name) {
#if defined(__ANDROID__)
	gator_annotate_name_channel(id, 0, name.ascii().get_data());
#endif
}

PerformanceStudio_Channel::PerformanceStudio_Channel() {
	countMutex->lock();
	id = ++count;
	countMutex->unlock();
}
