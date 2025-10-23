#include "Settings.h"

using namespace LindaEngine;

RenderQueueRange RenderQueueRange::all = { 0, 5000 };
RenderQueueRange RenderQueueRange::opaque = { 0, 2500 };
RenderQueueRange RenderQueueRange::transparent = { 2501, 5000 };
