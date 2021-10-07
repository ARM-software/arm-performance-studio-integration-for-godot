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

#ifndef PERFORMANCE_STUDIO_CHANNEL_H
#define PERFORMANCE_STUDIO_CHANNEL_H

#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

	class PerformanceStudio_Channel : public RefCounted {
		GDCLASS(PerformanceStudio_Channel, RefCounted)

	protected:
		static void _bind_methods();

	public:
		static void cleanup();

		/*
		 * Start an annotation in the channel. This will appear in the channel as an
		 * activity that begins when this method was called and ends when the next
		 * annotate() call is made, or the next end() call.
		 */
		void annotate(String str);

		/*
		 * As above, but with a specific color.
		 */
		void annotate_color(String str, Color color);

		/*
		 * Marks the end of an annotation.
		 */
		void end();

		void init(String name);
		PerformanceStudio_Channel();

	private:
		static uint32_t count;

		uint32_t id;
	};

}

#endif // PERFORMANCE_STUDIO_CHANNEL_H
