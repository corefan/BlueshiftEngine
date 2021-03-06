// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

/*
-------------------------------------------------------------------------------

    Render command

-------------------------------------------------------------------------------
*/

BE_NAMESPACE_BEGIN

const int RenderCommandBufferSize = 0x80000;

enum RenderCommand {
    EndOfCommand,
    BeginContextCommand,
    DrawViewCommand,
    ScreenShotCommand,
    SwapBuffersCommand
};

struct RenderCommandBuffer {
    byte                data[RenderCommandBufferSize];
    int                 used;
};

struct BeginContextRenderCommand {
    int                 commandId;
    RenderContext *     renderContext;
};

struct DrawViewRenderCommand {
    int                 commandId;
    view_t              view;
};

struct ScreenShotRenderCommand {
    int                 commandId;
    int                 x;
    int                 y;
    int                 width;
    int                 height;
    char                filename[MaxAbsolutePath];
};

struct SwapBuffersRenderCommand {
    int                 commandId;
};

BE_NAMESPACE_END
