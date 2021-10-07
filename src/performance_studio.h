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

#ifndef PERFORMANCE_STUDIO_H
#define PERFORMANCE_STUDIO_H

#include <godot_cpp/classes/ref_counted.hpp>
#include "performance_studio_cam.h"
#include "performance_studio_channel.h"
#include "performance_studio_counter.h"

namespace godot {

	class PerformanceStudio : public RefCounted {
		GDCLASS(PerformanceStudio, RefCounted)

	protected:
		static void _bind_methods();

	public:
		static void cleanup();

		/*
		 * Emit a simple marker that is displayed along the top of Streamline's
		 * timeline.
		 */
		void marker(String str);

		/*
		 * Emit a colored marker that is displayed along the top of Streamline's
		 * timeline.
		 */
		void marker_color(String str, Color color);

		/*
		 * Create a custom counter that can be tracked in Streamline.
		 */
		Ref<PerformanceStudio_Counter> create_counter(String title, String series, bool delta);

		/*
		 * Create a channel of activity, displayed as a row in Streamline's heat
		 * map view.
		 */
		Ref<PerformanceStudio_Channel> create_channel(String name);

		/*
		 * Create a Custom Activity Map, which is displayed in the bottom half of
		 * the Streamline UI.
		 */
		Ref<PerformanceStudio_CAM> create_cam(String name);

		PerformanceStudio();
	};

}

#endif // PERFORMANCE_STUDIO_H
