Originally, a separate mechanism to handle custom events or signals was envisioned.  
However, the current infrastructure should be enough to support them.

Each frame should have a special symbol `$handler` or something like that which can:
- Raise the event top (if not a slot and coming from down).
- Propagate down to all children except the direction it is coming from (if not a container and coming from up)
Events self-raised can be propagated top and down regardless of the self type.

If not implemented the handler will follow any direction encoded within the signal packet to determine where it should go.
Else, the custom implementation will take over, but the basic propagation functions are exposed for use.

## Signal packet
- Origin (ptr).
- Dest (ptr).
- From, the node this event was propagated from. It changes as the packet is propagated.
- Payload (buffer)
- Bubble up flag. Suggestion to propagate the event up.
- Propagate down flag. Suggestion to propagate the event down.
- Once flag. Suggestion to stop evaluation when the first match is found.
- Function to determine if a node should or should not be selected for propagation (a pre-filter)
- Maximum propagation level (to avoid looping them forever in strange cases)