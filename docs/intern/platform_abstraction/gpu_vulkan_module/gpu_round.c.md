# src/platform_abstraction/gpu_vulkan_module/gpu_round.c

This file takes an image of the window and shows it.

## The take

`round_begin` runs in the phase `first` of each round:

1. It waits on the timeline semaphore for the value `frame` minus `frames_in_flight`. The wait is host side, on the queue of graphics.
2. It asks for the next image with the semaphore unsignaled at the value of the round.
3. An `OUT_OF_DATE` marks the swapchain for a remake, and the round is skipped. `TIMEOUT` cannot happen: the wait is endless. Any other error writes the line of the swapchain and skips the round.
4. It records the barrier that moves the image to the layout for drawers, and submits it. The timeline is signaled at the value of the round.
5. It attaches `ideatek:Swapchain` and `ideatek:VulkanSwapchain` with `frame`, `slot` and the image of the round, and the status `IDEATEK_SWAPCHAIN_STATUS_READY`.

With no device, no surface, no swapchain, a minimized window or a size of 0, it attaches the values of the round.
The status of the swapchain is `IDEATEK_SWAPCHAIN_STATUS_SKIPPED` then.
No drawer draws then.

```pseudo
FUNCTION round_begin() RETURNS U32
   IF device = 0 OR swapchain = 0 OR height = 0 THEN
      CALL attach_skipped()
      RETURN OK
   ENDIF
   CALL wait_timeline(frame - frames)
   r <- vkAcquireNextImageKHR()
   IF r = OUT_OF_DATE THEN
      CALL mark_remake()
      CALL attach_skipped()
      RETURN OK
   ENDIF
   CALL submit_begin_barrier()
   CALL attach_round()
   RETURN OK
ENDFUNCTION
```

## The show

`round_present` runs in the phase `last`:

1. It records the barrier that moves the image to the layout of showing, and submits it. The submit signals the timeline with the value of the round.
2. It shows the image on the queue of graphics. An `OUT_OF_DATE` at the show marks the remake.
3. `frame` grows by 1.

The 2 barriers and the 2 submits use 1 command buffer of each frame from the pool of the module. No 2 drawers share a queue in a phase: the pairs of the deployment order them.

## The branches

| Case | Result |
| --- | --- |
| The wait of the timeline. | The objects of the frame are free. |
| The acquire gives `OUT_OF_DATE`. | A remake comes, and the round is skipped. |
| The window is minimized or of size 0. | The status `SKIPPED`, and no image is taken. |
| The show gives `OUT_OF_DATE`. | A remake comes in the next round. |
| A drawer left the image in its own layout. | The last barrier moves it back. |

## Functions

| Function | Result |
| --- | --- |
| `round_begin` | Waits, takes the image, submits the first barrier, and attaches the values of the round. |
| `round_present` | Submits the last barrier and shows the image. |
| `submit_barrier` | Records and submits 1 barrier, and signals the timeline. |
| `attach_round` | Attaches the values of the round on the entity of the window. |
