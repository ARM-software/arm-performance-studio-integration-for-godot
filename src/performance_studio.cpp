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

#include "performance_studio.h"
#include "helpers.h"
#include <godot_cpp/classes/mutex.hpp>

#if defined(__ANDROID__)
#include "streamline_annotate.h"
#endif

using namespace godot;

static bool initialized = false;
static Ref<Mutex> initializeMutex;

void PerformanceStudio::_bind_methods() {
	ClassDB::bind_method(D_METHOD("marker", "name"), &PerformanceStudio::marker);
	ClassDB::bind_method(D_METHOD("marker_color", "name", "color"), &PerformanceStudio::marker_color);
	ClassDB::bind_method(D_METHOD("create_counter", "title", "series", "delta"), &PerformanceStudio::create_counter);
	ClassDB::bind_method(D_METHOD("create_channel", "name"), &PerformanceStudio::create_channel);
	ClassDB::bind_method(D_METHOD("create_cam", "name"), &PerformanceStudio::create_cam);
	initializeMutex.instantiate();
}

void PerformanceStudio::cleanup() {
	initializeMutex.unref();
}

void PerformanceStudio::marker(String str) {
#if defined(__ANDROID__)
	gator_annotate_marker(str.ascii().get_data());
#endif
}

void PerformanceStudio::marker_color(String str, Color color) {
#if defined(__ANDROID__)
	uint32_t col = color_to_gator_int(color);
	gator_annotate_marker_color(col, str.ascii().get_data());
#endif
}

Ref<PerformanceStudio_Counter> PerformanceStudio::create_counter(String title, String series, bool delta) {
	Ref<PerformanceStudio_Counter> counter(memnew(PerformanceStudio_Counter));
	counter->init(title, series, delta);
	return counter;
}

Ref<PerformanceStudio_Channel> PerformanceStudio::create_channel(String name) {
	Ref<PerformanceStudio_Channel> channel(memnew(PerformanceStudio_Channel));
	channel->init(name);
	return channel;
}

Ref<PerformanceStudio_CAM> PerformanceStudio::create_cam(String name) {
	Ref<PerformanceStudio_CAM> cam(memnew(PerformanceStudio_CAM));
	cam->init(name);
	return cam;
}

PerformanceStudio::PerformanceStudio() {
#if defined(__ANDROID__)
	initializeMutex->lock();

	if (!initialized) {
		gator_annotate_setup();
		initialized = true;
	}

	initializeMutex->unlock();
#endif
}
