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

#include "performance_studio_cam.h"
#include "helpers.h"
#include <godot_cpp/classes/mutex.hpp>

#if defined(__ANDROID__)
#include "streamline_annotate.h"
#endif

using namespace godot;

void PerformanceStudio_CAMJob::_bind_methods() {
	ClassDB::bind_method(D_METHOD("stop"), &PerformanceStudio_CAMJob::stop);
}

void PerformanceStudio_CAMJob::stop() {
#if defined(__ANDROID__)
	uint64_t stop_time = gator_get_time();
	gator_cam_job_stop(cam, id, stop_time);
#endif
}

void PerformanceStudio_CAMJob::init(uint32_t cam, uint32_t track, uint32_t id, String name, Color color) {
	this->cam = cam;
	this->track = track;
	this->id = id;

#if defined(__ANDROID__)
	uint64_t start_time = gator_get_time();
	uint32_t col = color_to_gator_int(color);
	gator_cam_job_start(cam, id, name.ascii().get_data(), track, start_time, col);
#endif
}

PerformanceStudio_CAMJob::PerformanceStudio_CAMJob() {

}

void PerformanceStudio_CAMTrack::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_job", "name", "color"), &PerformanceStudio_CAMTrack::create_job);
}

Ref<PerformanceStudio_CAMJob> PerformanceStudio_CAMTrack::create_job(String name, Color color) {
	Ref<PerformanceStudio_CAMJob> job(memnew(PerformanceStudio_CAMJob));
	job->init(cam, id, ++(*job_count), name, color);
	return job;
}

void PerformanceStudio_CAMTrack::init(uint32_t cam, uint32_t id, String name, uint32_t* job_count) {
	this->cam = cam;
	this->id = id;
	this->job_count = job_count;

#if defined(__ANDROID__)
	gator_cam_track(cam, id, 0xffffffff, name.ascii().get_data());
#endif
}

PerformanceStudio_CAMTrack::PerformanceStudio_CAMTrack() {

}

uint32_t PerformanceStudio_CAM::count = 0;
static Ref<Mutex> countMutex;

void PerformanceStudio_CAM::_bind_methods() {
	ClassDB::bind_method(D_METHOD("create_track", "name"), &PerformanceStudio_CAM::create_track);
	countMutex.instantiate();
}

void PerformanceStudio_CAM::cleanup() {
	countMutex.unref();
}

Ref<PerformanceStudio_CAMTrack> PerformanceStudio_CAM::create_track(String name) {
	Ref<PerformanceStudio_CAMTrack> track(memnew(PerformanceStudio_CAMTrack));
	track->init(id, ++track_count, name, &job_count);
	return track;
}

void PerformanceStudio_CAM::init(String name) {
#if defined(__ANDROID__)
	gator_cam_view_name(id, name.ascii().get_data());
#endif
}

PerformanceStudio_CAM::PerformanceStudio_CAM() {
	countMutex->lock();
	id = ++count;
	countMutex->unlock();

	track_count = 0;
	job_count = 0;
}
