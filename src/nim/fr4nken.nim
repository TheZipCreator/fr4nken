import bindings
import sdl2
import graphics

proc nimPreinit() {.exportc: "nim_preinit".} =
  graphicsDataTag = fr_newDataTag();
proc nimInit() {.exportc: "nim_init".} =
  const _ = 0
proc nimDeinit() {.exportc: "nim_deinit".} =
  const _ = 0

proc sdlLoop {.exportc: "fr_sdlLoop".} =
  var
    win: WindowPtr
    rend: RendererPtr
    evt = sdl2.defaultEvent
  let
    renderEvt = fr_registryGet("event_render")[]
  win = createWindow("FR4NKEN", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN or SDL_WINDOW_RESIZABLE)
  rend = createRenderer(win, -1, Renderer_Accelerated or Renderer_PresentVsync or Renderer_TargetTexture)
  # create graphics object and put it in registry
  var graphics = createGraphics(win, rend)
  fr_registryPut("graphics", graphics)
  block loop:
    while true:
      while pollEvent(evt):
        if evt.kind == QuitEvent:
          break loop
      rend.setDrawColor 0, 0, 0, 255
      rend.clear
      # trigger render event
      block:
          let args = [makeObject(graphics)]
          discard fr_callMethod(renderEvt, "trigger", 1, cast[ptr UncheckedArray[Value]](addr args))
      rend.present
  destroy rend
  destroy win

