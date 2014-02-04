/*
===========================================================================
Daemon BSD Source Code
Copyright (c) 2013-2014, Daemon Developers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Daemon developers nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL DAEMON DEVELOPERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
===========================================================================
*/

#ifndef AUDIO_SAMPLE_H_
#define AUDIO_SAMPLE_H_

#include "ALObjects.h"

namespace Audio {

    template<typename T>
    class HandledResource {
        public:
            struct handleRecord_t {
                bool active;
                T* value;
            };

            static bool IsValidHandle(int handle) {
                return handle >= 0 and (unsigned)handle < handles.size() and handles[handle].active;
            }
            static T* FromHandle(int handle) {
                if (not IsValidHandle(handle)) {
                    return nullptr;
                }

                return handles[handle].value;
            }

            int GetHandle() {
                return handle;
            }

        protected:
            HandledResource(T* self) {
                if (not inactiveHandles.empty()) {
                    handle = inactiveHandles.back();
                    inactiveHandles.pop_back();
                    handles[handle] = {true, self};
                } else {
                    handle = handles.size();
                    handles.push_back({true, self});
                }
            }

            ~HandledResource() {
                handles[handle] = {false, nullptr};
                inactiveHandles.push_back(handle);
            }

        private:
            int handle;

            static std::vector<handleRecord_t> handles;
            static std::vector<int> inactiveHandles;
    };

    template<typename T>
    std::vector<typename HandledResource<T>::handleRecord_t> HandledResource<T>::handles;

    template<typename T>
    std::vector<int> HandledResource<T>::inactiveHandles;

    //TODO handle memory pressure: load and unload buffers automatically
    class Sample: public HandledResource<Sample> {
        public:
            Sample(std::string name);
            ~Sample();

            void Use();

            AL::Buffer& GetBuffer();

            const std::string& GetName();

        private:
            AL::Buffer buffer;
            std::string name;
    };

    void InitSamples();
    void ShutdownSamples();

	std::vector<std::string> ListSamples();

    Sample* RegisterSample(Str::StringRef filename);
}

#endif //AUDIO_SAMPLE_H_