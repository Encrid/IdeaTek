# include/engine_subsystems/ideatek_ui.h

`include/engine_subsystems/ideatek_ui.h` gives the types of the user interface: roots, nodes, layout, looks, text, pointers, focus, widgets, screens, gestures, world markers and styles.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the rendering part for the colors of the looks.

## The roles

- The game is the author. It writes the roots, the nodes and their types, the widgets, the screens and the styles.
- The UI module lays out the nodes, tests the pointers, moves the focus, runs the widgets and draws the trees into their targets.
- Only the game writes a value of a widget. The UI module proposes each change as an event, and the game writes the value that it accepts.

## The roots and the units

A tree of nodes starts at an entity with `ideatek:UiRoot`. Its target is a window entity, or a texture entity for a surface in the world.
The scale of a root is `display_scale` of the window times `scale`. A texture uses `scale` alone. A root on no valid target does not draw, and the UI module logs it once.

A UI unit is a pixel of the target divided by the scale. The size of a root in UI units is the size of its target divided by its scale.
A root of a higher `order` draws on top and gets the pointer first.
With `right_to_left`, a row runs from the right, START is on the right, and the text runs from right to left.

## The layout

A node is an entity with `ideatek:UiNode` under a root, by `ideatek:Parent` of the space part. `order` ranks the children of 1 parent. It is also the order of the draw and of the hits.
The game writes `ideatek:UiLayout`. The layout module alone writes `ideatek:UiRect`, in UI units from the origin of the root.

The layout is a subset of the flex layout of CSS. An absolute node with lengths in percent is placed by anchors.
The module keeps `ideatek:UiScroll` inside the size of the content. It measures each text node, so a size of `IDEATEK_UI_UNIT_AUTO` fits the text.

## The looks

`ideatek:UiBox` draws a background, a border and a shadow. The border takes the widths of `ideatek:UiLayout`. A color with an alpha of 0 does not draw.
`ideatek:UiImage` draws over the background and inside the border. A second shadow needs a child node. `ideatek:UiGradient` draws over the background and under the image.

`ideatek:UiMask` clips the children by the alpha of its texture, or by the box of the node with its corner radii, for a round map. The overflow CLIP stays a rectangle.

## The text

`ideatek:UiText` holds the string, and `ideatek:UiTextLook` holds its look. A span on a child entity changes a part of the string. The spans follow `order`.
A font is a resource entity with `ideatek:Font`. 1 file is 1 face. When a font lacks a glyph, the module takes it from the fallback, and then from the fallback of the fallback.

A span with an `image` draws the texture at the height of the line, in place of its text. The text stays, for a screen reader.
`letter_spacing` adds space between the glyphs. With ELLIPSIS, a line that is too long ends with an ellipsis.
A text node with `ideatek:LocalizedText` of the localization part draws that text in place of the text of `ideatek:UiText`.

The module that writes `ideatek:UiRect` shapes and measures the text. Its glyphs, its atlas and the method of the draw are its own.

## The pointers

Each pointer is an entity. The UI module writes 1 pointer for the mouse of each window, and 1 pointer for each finger. It reads only the finger events for a touch.
The game writes the pointer of a texture root, for example from a ray and the UV that it hits.
Only a node with `ideatek:UiPickable` gets hits. A node without it gives its hits to its parent, so no event bubbles.

A press keeps its node until the release. A click is a press and a release on 1 node, with no drag.

## The focus

A focus entity is the focus of 1 player. Its actions are actions of the map of the inputs, so a rebinding also changes the menus.
The focus moves only inside its root. A focus with a root of 0 follows its top screen.
With no node, the focus takes the first AUTO node of its root, or else the first node by `tab`. A neighbor of 0 is the nearest node in that direction, and the node itself is a wall.

`visible` turns on after a move by an action, and off after a press of a pointer.

## The widgets

A widget proposes a value with `ideatek:UiValueEvent`, and the game writes `ideatek:UiValue`. So the game can clamp a value, refuse it or save it.
A toggle proposes the other value. In a group, it proposes 1 for itself and 0 for the toggle that had 1. A check box is a toggle with no group.

A slider with the focus takes the left and right actions, or the up and down actions when it is vertical. The game places the thumb from `ideatek:UiValue`.
With `ideatek:UiScrollArea`, the UI module is the only writer of `ideatek:UiScroll` on the node.
A text field with the focus asks the input part for text, and proposes each change with `ideatek:UiTextChange`.
A drop-down is a toggle that opens a screen with a list. A disabled widget gives no event, and it keeps its hits and its focus.

## The screens

A screen is a root with `ideatek:UiScreen`. The screens of 1 focus are a stack, and the top screen is the screen of the highest `order`.
To push a screen, the game spawns a root of a higher order. To pop it, the game destroys it. A back press is the cancel action of the focus.
A modal top screen takes the hits of the roots of a lower order on its target, even where it has no node.

The game reads `ideatek:UiScreenEvent` and turns the context of the inputs of each screen on or off. So the game stays the only writer of the contexts.

## The gestures

The UI module finds the gestures in the finger pointers. A tap ends before `long_press`, with less motion than `drag_threshold`.
A pan moves more than `drag_threshold`. A pinch has 2 fingers and gives the scale and the angle since its start.
A swipe is the end of a pan with its velocity, and the game chooses its own speed limit.

## The grid

A node with `IDEATEK_UI_DISPLAY_GRID` places its children in the equal cells of `ideatek:UiGrid`, with `gap_x` and `gap_y` between them. A `rows` of 0 adds rows as needed.
A cell starts at 1, as a line of CSS grid. A child with `ideatek:UiGridItem` takes its cell and its spans. A column or a row of 0, or no `ideatek:UiGridItem`, takes the next free cell.

## The transforms and the opacity

`ideatek:UiTransform` moves, scales and turns the draw and the hits of a node and its children, about its pivot. It does not change the layout.
The pivot is a part of the size of the node, from 0 to 1. A pivot of 0.5 is the default of `transform-origin` of CSS and of RmlUi, as for a marker.
`rotation_x` and `rotation_y` turn the node in 3D about its pivot. A `perspective` above 0 is the distance of the eye, in UI units.

The hits go through the inverse of the transform. `ideatek:UiOpacity` multiplies the alpha of a node and of its children.

## The accessibility

`ideatek:UiAccessible` gives a node a label and a description for a screen reader. The UI module gives the tree to the screen reader of the OS, for example through AccessKit.
The role of a node comes from its widget: a button, a toggle, a slider, a scroll area or a text field. A toggle in a group is a radio button, and a toggle with no group is a check box.
A text node is a label. Each other node is a group.

## The shaders and the blur

With `ideatek:UiShader`, the render module draws the box of the node with its own shader of the name `name`. `params` holds the bytes that the shader reads.
A render module that does not know the name draws the box without the shader, and logs it once.
`ideatek:UiBlur` with `backdrop` at `true` blurs what is behind the box, as frosted glass. With `false`, it blurs the node and its children.
The references do not agree on a radius of the blur, so the game sets it.

## The animations

A node can hold a player of the animation part. The UI module reads `ideatek:AnimationCurves` on the node, and applies each curve of the table to the draw.
The id of a curve is the FNV-1a hash of 64 bits of its name, as the animation part says. The value that the game wrote stays in its type.
A play with `real_time` runs in a pause, so a menu of the pause can move. A loader of CSS turns `@keyframes` into a clip of these curves.

| Name | The value |
| --- | --- |
| `ui.x`, `ui.y` | `x` and `y` of `ideatek:UiTransform` |
| `ui.scale_x`, `ui.scale_y` | `scale_x` and `scale_y` of `ideatek:UiTransform` |
| `ui.rotation`, `ui.rotation_x`, `ui.rotation_y` | The turns of `ideatek:UiTransform` |
| `ui.opacity` | `value` of `ideatek:UiOpacity` |
| `ui.background.r`, `ui.background.g`, `ui.background.b`, `ui.background.a` | `background` of `ideatek:UiBox` |
| `ui.border.r`, `ui.border.g`, `ui.border.b`, `ui.border.a` | `border` of `ideatek:UiBox` |
| `ui.tint.r`, `ui.tint.g`, `ui.tint.b`, `ui.tint.a` | `tint` of `ideatek:UiImage` |
| `ui.text.r`, `ui.text.g`, `ui.text.b`, `ui.text.a` | `color` of `ideatek:UiTextLook` |

## The markers

A node with `ideatek:UiAnchor` follows an entity of the world through a camera. The layout puts the pivot of the node at the point, and does not read the left and top of the node.
With HIDE_BEHIND, a point behind the camera hides the node, and it takes no hits.

## The styles

A node with `ideatek:UiStyle` takes its looks from a style entity for each state. A style entity holds `ideatek:UiBox`, `ideatek:UiImage`, `ideatek:UiTextLook`, `ideatek:UiGradient`, `ideatek:UiOpacity` and `ideatek:UiBlur`.
The state is disabled, then pressed, then hovered, then normal. A toggle with a value of 1 is pressed.
While the node has the focus and the focus is visible, the looks of `focus` draw over the look of the state.
The modules read the looks at the time of the draw, and copy nothing, so each look keeps 1 writer.

At a change of state, the colors and the numbers of the looks blend over `transition`, along the curve of `ease` and `ease_direction`.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `drag_threshold` | 10 | 0 or more | The motion, in pixels, before a press becomes a drag. |
| `long_press` | 0.5 | Above 0 | The seconds of a long press. |

## ideatek:UiRoot

`ideatek:UiRoot` starts a tree of nodes on a target.
`ideatek_ui_root_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `target` | `ideatek_entity_t` | 0 | No default | A window, or a texture. |
| `order` | `int32_t` | 8 | 0 | The rank of the root on its target. |
| `scale` | `float` | 12 | 1 | The factor of the size of the UI. |
| `right_to_left` | `bool` | 16 | `false` | `true` if rows and text run from the right. |

Bytes 17 to 23 hold no field.

The UI size of the settings of a game is `scale`. A curve of the DPI is a system of the game that writes `scale`.

## ideatek:UiNode

`ideatek:UiNode` makes an entity a node of a tree.
`ideatek_ui_node_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `order` | `int32_t` | 0 | 0 | The rank among the children. |

## ideatek:ideatek_ui_unit_t

`ideatek_ui_unit_t` tells the unit of a length.
`ideatek_ui_unit_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_UI_UNIT_AUTO` | The layout chooses. |
| 1 | `IDEATEK_UI_UNIT_PX` | UI units. |
| 2 | `IDEATEK_UI_UNIT_PERCENT` | A percent of the parent. |

## ideatek:ideatek_ui_length_t

`ideatek_ui_length_t` is a length of the layout. A value of zeros is AUTO.
`ideatek_ui_length_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `value` | `float` | 0 | The number. |
| `unit` | `ideatek_ui_unit_t` | 4 | The unit. |

Bytes 5 to 7 hold no field.

## ideatek:ideatek_ui_display_t

`ideatek_ui_display_t` tells the layout of a node.
`ideatek_ui_display_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_UI_DISPLAY_NONE` | The node and its children are not there. |
| 1 | `IDEATEK_UI_DISPLAY_FLEX` | The flex layout of CSS. |
| 2 | `IDEATEK_UI_DISPLAY_GRID` | A grid of equal cells: *The grid*. |

## ideatek:ideatek_ui_position_t

`ideatek_ui_position_t` tells how a node is placed.
`ideatek_ui_position_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_UI_POSITION_RELATIVE` | In the flow of the parent. |
| 1 | `IDEATEK_UI_POSITION_ABSOLUTE` | Out of the flow, at its left, right, top and bottom. |

## ideatek:ideatek_ui_direction_t

`ideatek_ui_direction_t` tells the main axis of a node.
`ideatek_ui_direction_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_UI_DIRECTION_ROW` | The children go to the right. |
| 1 | `IDEATEK_UI_DIRECTION_COLUMN` | The children go down. |
| 2 | `IDEATEK_UI_DIRECTION_ROW_REVERSE` | The children go to the left. |
| 3 | `IDEATEK_UI_DIRECTION_COLUMN_REVERSE` | The children go up. |

## ideatek:ideatek_ui_align_t

`ideatek_ui_align_t` tells an alignment of the flex layout, as in CSS.
`ideatek_ui_align_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_UI_ALIGN_START` | At the start. |
| 1 | `IDEATEK_UI_ALIGN_CENTER` | At the center. |
| 2 | `IDEATEK_UI_ALIGN_END` | At the end. |
| 3 | `IDEATEK_UI_ALIGN_STRETCH` | Over the whole space. |
| 4 | `IDEATEK_UI_ALIGN_SPACE_BETWEEN` | The space goes between the children. |
| 5 | `IDEATEK_UI_ALIGN_SPACE_AROUND` | The space goes around each child. |
| 6 | `IDEATEK_UI_ALIGN_SPACE_EVENLY` | The space is equal everywhere. |

## ideatek:ideatek_ui_overflow_t

`ideatek_ui_overflow_t` tells what a node does with content that is too large.
`ideatek_ui_overflow_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_UI_OVERFLOW_VISIBLE` | The content shows outside. |
| 1 | `IDEATEK_UI_OVERFLOW_CLIP` | The content is cut at the edge. |
| 2 | `IDEATEK_UI_OVERFLOW_SCROLL` | The content is cut and scrolls. |

## ideatek:UiLayout

`ideatek:UiLayout` gives the rules of the place and the size of a node: *The layout*.
`ideatek_ui_layout_t` takes 168 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `left` | `ideatek_ui_length_t` | 0 | No default | The place of the left edge. |
| `right` | `ideatek_ui_length_t` | 8 | No default | The place of the right edge. |
| `top` | `ideatek_ui_length_t` | 16 | No default | The place of the top edge. |
| `bottom` | `ideatek_ui_length_t` | 24 | No default | The place of the bottom edge. |
| `width` | `ideatek_ui_length_t` | 32 | No default | The width. |
| `height` | `ideatek_ui_length_t` | 40 | No default | The height. |
| `min_width` | `ideatek_ui_length_t` | 48 | No default | The smallest width. |
| `min_height` | `ideatek_ui_length_t` | 56 | No default | The smallest height. |
| `max_width` | `ideatek_ui_length_t` | 64 | No default | The largest width. |
| `max_height` | `ideatek_ui_length_t` | 72 | No default | The largest height. |
| `basis` | `ideatek_ui_length_t` | 80 | No default | The size before grow and shrink. |
| `aspect` | `float` | 88 | 0 | The width divided by the height. 0 means none. |
| `margin_left` | `float` | 92 | No default | The space outside, on the left. |
| `margin_right` | `float` | 96 | No default | The space outside, on the right. |
| `margin_top` | `float` | 100 | No default | The space outside, at the top. |
| `margin_bottom` | `float` | 104 | No default | The space outside, at the bottom. |
| `padding_left` | `float` | 108 | No default | The space inside, on the left. |
| `padding_right` | `float` | 112 | No default | The space inside, on the right. |
| `padding_top` | `float` | 116 | No default | The space inside, at the top. |
| `padding_bottom` | `float` | 120 | No default | The space inside, at the bottom. |
| `border_left` | `float` | 124 | No default | The width of the left border. |
| `border_right` | `float` | 128 | No default | The width of the right border. |
| `border_top` | `float` | 132 | No default | The width of the top border. |
| `border_bottom` | `float` | 136 | No default | The width of the bottom border. |
| `grow` | `float` | 140 | 0 | The part of the free space that it takes. |
| `shrink` | `float` | 144 | 1 | The part of the lack of space that it gives. |
| `gap_x` | `float` | 148 | No default | The space between columns. |
| `gap_y` | `float` | 152 | No default | The space between rows. |
| `display` | `ideatek_ui_display_t` | 156 | `IDEATEK_UI_DISPLAY_FLEX` | The layout. |
| `position` | `ideatek_ui_position_t` | 157 | No default | The place. |
| `direction` | `ideatek_ui_direction_t` | 158 | No default | The main axis. |
| `wrap` | `bool` | 159 | `false` | `true` if the children wrap to new lines. |
| `align_items` | `ideatek_ui_align_t` | 160 | No default | The children on the cross axis. |
| `align_self` | `ideatek_ui_align_t` | 161 | No default | This node on the cross axis of its parent. |
| `justify_content` | `ideatek_ui_align_t` | 162 | No default | The children on the main axis. |
| `align_content` | `ideatek_ui_align_t` | 163 | No default | The lines on the cross axis. |
| `overflow_x` | `ideatek_ui_overflow_t` | 164 | No default | Content too wide. |
| `overflow_y` | `ideatek_ui_overflow_t` | 165 | No default | Content too high. |

Bytes 166 to 167 hold no field.

The margins, the paddings, the borders and the gaps are in UI units. The border box holds the padding and the border.

## ideatek:UiScroll

`ideatek:UiScroll` scrolls the content of a node with an overflow of SCROLL.
`ideatek_ui_scroll_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The scroll to the right. |
| `y` | `float` | 4 | The scroll down. |

## ideatek:UiRect

`ideatek:UiRect` is the result of the layout of a node. Only the layout module writes it.
`ideatek_ui_rect_t` takes 40 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The left edge. |
| `y` | `float` | 4 | The top edge. |
| `width` | `float` | 8 | The width. |
| `height` | `float` | 12 | The height. |
| `content_width` | `float` | 16 | The width of the content. |
| `content_height` | `float` | 20 | The height of the content. |
| `clip_x` | `float` | 24 | The left edge of the clip. |
| `clip_y` | `float` | 28 | The top edge of the clip. |
| `clip_width` | `float` | 32 | The width of the clip. |
| `clip_height` | `float` | 36 | The height of the clip. |

## ideatek:ideatek_flip_t

`ideatek_flip_t` names the mirrors of an image.
`ideatek_flip_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_FLIP_X` | A mirror on X. |
| 0x02 | `IDEATEK_FLIP_Y` | A mirror on Y. |

## ideatek:ideatek_image_mode_t

`ideatek_image_mode_t` tells how an image fills a node.
`ideatek_image_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_IMAGE_MODE_STRETCH` | The image fills the node. |
| 1 | `IDEATEK_IMAGE_MODE_SLICED` | The corners stay, and the edges and the center stretch. |
| 2 | `IDEATEK_IMAGE_MODE_TILED` | The image repeats. |

## ideatek:UiBox

`ideatek:UiBox` draws the box of a node.
`ideatek_ui_box_t` takes 80 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `background` | `ideatek_color_t` | 0 | The color of the inside. |
| `border` | `ideatek_color_t` | 16 | The color of the border. |
| `shadow` | `ideatek_color_t` | 32 | The color of the shadow. |
| `radius` | `float[4]` | 48 | The radii of the top left, top right, bottom right and bottom left corners. |
| `shadow_x` | `float` | 64 | The offset of the shadow on X. |
| `shadow_y` | `float` | 68 | The offset of the shadow on Y. |
| `shadow_blur` | `float` | 72 | The blur of the shadow. |
| `shadow_spread` | `float` | 76 | The growth of the shadow. |

## ideatek:UiImage

`ideatek:UiImage` draws an image in a node.
`ideatek_ui_image_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | No default | The texture. |
| `tint` | `ideatek_color_t` | 8 | 1, 1, 1, 1 | The factor of the color. |
| `uv_x` | `float` | 24 | No default | The left of the part of the texture, in texels. |
| `uv_y` | `float` | 28 | No default | The top of that part. |
| `uv_width` | `float` | 32 | No default | The width of that part. 0 means the whole texture. |
| `uv_height` | `float` | 36 | No default | The height of that part. |
| `slice_left` | `float` | 40 | No default | The left edge of a slice, in texels. |
| `slice_top` | `float` | 44 | No default | The top edge of a slice. |
| `slice_right` | `float` | 48 | No default | The right edge of a slice. |
| `slice_bottom` | `float` | 52 | No default | The bottom edge of a slice. |
| `mode` | `ideatek_image_mode_t` | 56 | `IDEATEK_IMAGE_MODE_STRETCH` | The fill. |
| `flip` | `ideatek_flip_t` | 57 | 0 | The mirrors. |

Bytes 58 to 63 hold no field.

## ideatek:ideatek_font_format_t

`ideatek_font_format_t` tells the format of a font.
`ideatek_font_format_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FONT_FORMAT_SFNT` | A TrueType or an OpenType file. |

## ideatek:Font

`ideatek:Font` is 1 face of a font on a resource entity.
`ideatek_font_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `fallback` | `ideatek_entity_t` | 0 | The next font for a glyph that this font lacks. 0 means none. |
| `bytes` | `bridge_text_t` | 8 | The bytes of the file. |
| `revision` | `uint32_t` | 16 | Raised at each change. |
| `format` | `ideatek_font_format_t` | 20 | The format. |

Bytes 21 to 23 hold no field.

## ideatek:UiText

`ideatek:UiText` puts a string in a node.
`ideatek_ui_text_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `bridge_text_t` | 0 | The string, in UTF-8. |

## ideatek:ideatek_text_align_t

`ideatek_text_align_t` tells the place of the lines of a text.
`ideatek_text_align_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TEXT_ALIGN_START` | Each line at the start. |
| 1 | `IDEATEK_TEXT_ALIGN_CENTER` | Each line at the center. |
| 2 | `IDEATEK_TEXT_ALIGN_END` | Each line at the end. |
| 3 | `IDEATEK_TEXT_ALIGN_JUSTIFY` | Each line but the last fills the width. |

## ideatek:ideatek_text_wrap_t

`ideatek_text_wrap_t` tells where a text breaks.
`ideatek_text_wrap_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TEXT_WRAP_WORD` | A new line between words. |
| 1 | `IDEATEK_TEXT_WRAP_ANY` | A new line between any characters. |
| 2 | `IDEATEK_TEXT_WRAP_NONE` | No new line. |

## ideatek:ideatek_text_overflow_t

`ideatek_text_overflow_t` tells the end of a line that is too long, as `text-overflow` of CSS.
`ideatek_text_overflow_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TEXT_OVERFLOW_CLIP` | The line is cut at the edge. |
| 1 | `IDEATEK_TEXT_OVERFLOW_ELLIPSIS` | The line ends with an ellipsis. |

## ideatek:UiTextLook

`ideatek:UiTextLook` is the look of a text, on the node or on a style entity.
`ideatek_ui_text_look_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `font` | `ideatek_entity_t` | 0 | No default | The font. |
| `color` | `ideatek_color_t` | 8 | 1, 1, 1, 1 | The color of the glyphs. |
| `outline_color` | `ideatek_color_t` | 24 | 0, 0, 0, 1 | The color of the outline. |
| `shadow` | `ideatek_color_t` | 40 | 0, 0, 0, 0 | The color of the shadow. An alpha of 0 turns it off. |
| `size` | `float` | 56 | 16 | The size, in UI units. |
| `line_height` | `float` | 60 | 1.2 | The height of a line, as a factor of the size. |
| `outline` | `float` | 64 | 0 | The width of the outline. |
| `shadow_x` | `float` | 68 | 1 | The offset of the shadow on X. |
| `shadow_y` | `float` | 72 | 1 | The offset of the shadow on Y. |
| `letter_spacing` | `float` | 76 | 0 | The space between glyphs, in UI units. |
| `align` | `ideatek_text_align_t` | 80 | `IDEATEK_TEXT_ALIGN_START` | The place of the lines. |
| `wrap` | `ideatek_text_wrap_t` | 81 | `IDEATEK_TEXT_WRAP_WORD` | The breaks. |
| `overflow` | `ideatek_text_overflow_t` | 82 | `IDEATEK_TEXT_OVERFLOW_CLIP` | The end of a line that is too long. |

Bytes 83 to 87 hold no field.

A size of 16 is the default of Godot and of CSS. A line height of 1.2 is the default of Bevy and of CSS.

## ideatek:UiTextSpan

`ideatek:UiTextSpan` is a part of a text with its own look, on a child entity of a text node.
`ideatek_ui_text_span_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `bridge_text_t` | 0 | The string of the span. |
| `font` | `ideatek_entity_t` | 8 | The font. 0 means the font of the parent. |
| `image` | `ideatek_entity_t` | 16 | A texture in place of the text. 0 means none. |
| `color` | `ideatek_color_t` | 24 | The color. An alpha of 0 means the color of the parent. |
| `size` | `float` | 40 | The size. 0 means the size of the parent. |

Bytes 44 to 47 hold no field.

## ideatek:ideatek_pointer_buttons_t

`ideatek_pointer_buttons_t` names the buttons of a pointer.
`ideatek_pointer_buttons_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_POINTER_BUTTONS_PRIMARY` | The primary button, or a finger. |
| 0x02 | `IDEATEK_POINTER_BUTTONS_SECONDARY` | The secondary button. |
| 0x04 | `IDEATEK_POINTER_BUTTONS_MIDDLE` | The middle button. |

## ideatek:UiPointer

`ideatek:UiPointer` makes an entity a pointer: *The pointers*.
`ideatek_ui_pointer_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `target` | `ideatek_entity_t` | 0 | The window or the texture. |
| `x` | `float` | 8 | The position on X, in pixels of the target. |
| `y` | `float` | 12 | The position on Y. |
| `wheel_x` | `float` | 16 | The wheel on X in the round. |
| `wheel_y` | `float` | 20 | The wheel on Y in the round. |
| `buttons` | `ideatek_pointer_buttons_t` | 24 | The buttons that are down. |

Bytes 25 to 31 hold no field.

## ideatek:UiPointerHit

`ideatek:UiPointerHit` names the node under a pointer. The game reads it to block the input of the world.
`ideatek_ui_pointer_hit_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The top node under the pointer. 0 means none. |

## ideatek:ideatek_pickable_flags_t

`ideatek_pickable_flags_t` names the options of a node for the pointers.
`ideatek_pickable_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_PICKABLE_FLAGS_BLOCK` | The node blocks the nodes and the world below it. |
| 0x02 | `IDEATEK_PICKABLE_FLAGS_HOVER` | The node gets events. |

## ideatek:UiPickable

`ideatek:UiPickable` makes a node get the hits of the pointers.
`ideatek_ui_pickable_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `flags` | `ideatek_pickable_flags_t` | 0 | BLOCK and HOVER | The options. |

## ideatek:ideatek_interaction_t

`ideatek_interaction_t` tells what a pointer does to a node.
`ideatek_interaction_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_INTERACTION_NONE` | No pointer. |
| 1 | `IDEATEK_INTERACTION_HOVERED` | A pointer is over the node. |
| 2 | `IDEATEK_INTERACTION_PRESSED` | A pointer presses the node. |

## ideatek:UiInteraction

`ideatek:UiInteraction` is the state of a node for the pointers. The UI module writes it.
`ideatek_ui_interaction_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `pointer` | `ideatek_entity_t` | 0 | The pointer. |
| `state` | `ideatek_interaction_t` | 8 | What it does. |

Bytes 9 to 15 hold no field.

## ideatek:ideatek_pointer_event_kind_t

`ideatek_pointer_event_kind_t` tells the kind of a pointer event.
`ideatek_pointer_event_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_POINTER_EVENT_KIND_ENTER` | The pointer comes over the node. |
| 1 | `IDEATEK_POINTER_EVENT_KIND_LEAVE` | The pointer leaves the node. |
| 2 | `IDEATEK_POINTER_EVENT_KIND_PRESS` | A button goes down. |
| 3 | `IDEATEK_POINTER_EVENT_KIND_RELEASE` | A button goes up. |
| 4 | `IDEATEK_POINTER_EVENT_KIND_CLICK` | A press and a release on the node. |
| 5 | `IDEATEK_POINTER_EVENT_KIND_DRAG_START` | A drag starts. |
| 6 | `IDEATEK_POINTER_EVENT_KIND_DRAG` | A drag moves. |
| 7 | `IDEATEK_POINTER_EVENT_KIND_DRAG_END` | A drag ends. |
| 8 | `IDEATEK_POINTER_EVENT_KIND_DROP` | A drag ends on this node. |
| 9 | `IDEATEK_POINTER_EVENT_KIND_SCROLL` | The wheel turns over the node. |

## ideatek:UiPointerEvent

`ideatek:UiPointerEvent` tells what a pointer did to a node. It is an event of 1 round.
`ideatek_ui_pointer_event_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The node. |
| `pointer` | `ideatek_entity_t` | 8 | The pointer. |
| `other` | `ideatek_entity_t` | 16 | For a drop, the dragged node. |
| `x` | `float` | 24 | The position on X, in UI units of the node. |
| `y` | `float` | 28 | The position on Y. |
| `dx` | `float` | 32 | The motion on X. |
| `dy` | `float` | 36 | The motion on Y. |
| `order` | `uint32_t` | 40 | The rank in the round. |
| `kind` | `ideatek_pointer_event_kind_t` | 44 | The kind. |
| `button` | `ideatek_pointer_buttons_t` | 45 | The button. |

Bytes 46 to 47 hold no field.

## ideatek:UiFocus

`ideatek:UiFocus` is the focus of 1 player: *The focus*.
`ideatek_ui_focus_t` takes 72 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `root` | `ideatek_entity_t` | 0 | The root. 0 means the top screen. |
| `up` | `ideatek_entity_t` | 8 | The action of a move up. |
| `down` | `ideatek_entity_t` | 16 | The action of a move down. |
| `left` | `ideatek_entity_t` | 24 | The action of a move left. |
| `right` | `ideatek_entity_t` | 32 | The action of a move right. |
| `next` | `ideatek_entity_t` | 40 | The action of the next node. |
| `previous` | `ideatek_entity_t` | 48 | The action of the node before. |
| `accept` | `ideatek_entity_t` | 56 | The action that activates. |
| `cancel` | `ideatek_entity_t` | 64 | The action that goes back. |

A held direction repeats with a pulse trigger on its bindings.

## ideatek:UiFocusState

`ideatek:UiFocusState` is the state of a focus. Only the UI module writes it.
`ideatek_ui_focus_state_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The node with the focus. 0 means none. |
| `visible` | `bool` | 8 | `true` if the focus shows. |

Bytes 9 to 15 hold no field.

## ideatek:ideatek_focusable_flags_t

`ideatek_focusable_flags_t` names how a node takes the focus.
`ideatek_focusable_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_FOCUSABLE_FLAGS_CLICK` | A press of a pointer gives the focus. |
| 0x02 | `IDEATEK_FOCUSABLE_FLAGS_NAVIGATE` | A move by an action can give the focus. |
| 0x04 | `IDEATEK_FOCUSABLE_FLAGS_AUTO` | The node takes the focus when its root has none. |

## ideatek:UiFocusable

`ideatek:UiFocusable` lets a node take the focus. A node with flags of 0 is skipped.
`ideatek_ui_focusable_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `tab` | `int32_t` | 0 | 0 | The rank of next and previous. |
| `flags` | `ideatek_focusable_flags_t` | 4 | CLICK and NAVIGATE | How it takes the focus. |

Bytes 5 to 7 hold no field.

## ideatek:UiNeighbors

`ideatek:UiNeighbors` names the next nodes of a node for each direction.
`ideatek_ui_neighbors_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `up` | `ideatek_entity_t` | 0 | The node above. |
| `down` | `ideatek_entity_t` | 8 | The node below. |
| `left` | `ideatek_entity_t` | 16 | The node on the left. |
| `right` | `ideatek_entity_t` | 24 | The node on the right. |

## ideatek:UiFocusMove

`ideatek:UiFocusMove` moves a focus. The game spawns it as an event.
`ideatek_ui_focus_move_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `focus` | `ideatek_entity_t` | 0 | The focus. |
| `node` | `ideatek_entity_t` | 8 | The new node. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:ideatek_focus_event_kind_t

`ideatek_focus_event_kind_t` tells the kind of a focus event.
`ideatek_focus_event_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FOCUS_EVENT_KIND_GAINED` | The node took the focus. |
| 1 | `IDEATEK_FOCUS_EVENT_KIND_LOST` | The node lost it. |
| 2 | `IDEATEK_FOCUS_EVENT_KIND_ACTIVATE` | The accept action on the node. |
| 3 | `IDEATEK_FOCUS_EVENT_KIND_CANCEL` | The cancel action on the node. |

## ideatek:ideatek_focus_cause_t

`ideatek_focus_cause_t` tells the cause of a change of the focus.
`ideatek_focus_cause_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FOCUS_CAUSE_NAVIGATE` | A move by an action. |
| 1 | `IDEATEK_FOCUS_CAUSE_POINTER` | A press of a pointer. |
| 2 | `IDEATEK_FOCUS_CAUSE_AUTO` | The UI module chose the node. |
| 3 | `IDEATEK_FOCUS_CAUSE_GAME` | A move of the game. |

## ideatek:UiFocusEvent

`ideatek:UiFocusEvent` tells a change of a focus. It is an event of 1 round.
`ideatek_ui_focus_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `focus` | `ideatek_entity_t` | 0 | The focus. |
| `node` | `ideatek_entity_t` | 8 | The node. |
| `order` | `uint32_t` | 16 | The rank in the round. |
| `kind` | `ideatek_focus_event_kind_t` | 20 | The kind. |
| `cause` | `ideatek_focus_cause_t` | 21 | The cause. |

Bytes 22 to 23 hold no field.

## ideatek:UiValue

`ideatek:UiValue` is the value of a widget. Only the game writes it.
`ideatek_ui_value_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `value` | `float` | 0 | The value. |

## ideatek:UiValueEvent

`ideatek:UiValueEvent` proposes a new value of a widget.
`ideatek_ui_value_event_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The widget. |
| `value` | `float` | 8 | The proposed value. |
| `order` | `uint32_t` | 12 | The rank in the round. |

## ideatek:ideatek_button_mode_t

`ideatek_button_mode_t` tells when a button activates.
`ideatek_button_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_BUTTON_MODE_RELEASE` | The button activates on the release. |
| 1 | `IDEATEK_BUTTON_MODE_PRESS` | The button activates on the press. |

## ideatek:UiButton

`ideatek:UiButton` makes a node a button.
`ideatek_ui_button_t` takes 2 bytes. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `mode` | `ideatek_button_mode_t` | 0 | `IDEATEK_BUTTON_MODE_RELEASE` | When it activates. |
| `disabled` | `bool` | 1 | `false` | `true` if it gives no event. |

## ideatek:UiActivated

`ideatek:UiActivated` tells that a button activated.
`ideatek_ui_activated_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The button. |
| `source` | `ideatek_entity_t` | 8 | The pointer or the focus. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:UiToggle

`ideatek:UiToggle` makes a node a toggle with a value of 0 or 1.
`ideatek_ui_toggle_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `group` | `ideatek_entity_t` | 0 | The group. 0 means none. |
| `disabled` | `bool` | 8 | `true` if it gives no event. |

Bytes 9 to 15 hold no field.

## ideatek:UiToggleGroup

`ideatek:UiToggleGroup` makes its toggles a radio group or a list of 1 choice.
`ideatek_ui_toggle_group_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `allow_none` | `bool` | 0 | `false` | `true` if no toggle can have 1. |

## ideatek:ideatek_slider_axis_t

`ideatek_slider_axis_t` tells the axis of a slider.
`ideatek_slider_axis_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SLIDER_AXIS_HORIZONTAL` | Left to right. |
| 1 | `IDEATEK_SLIDER_AXIS_VERTICAL` | Bottom to top. |

## ideatek:UiSlider

`ideatek:UiSlider` makes a node a slider.
`ideatek_ui_slider_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `min` | `float` | 0 | 0 | The lowest value. |
| `max` | `float` | 4 | 1 | The highest value. |
| `step` | `float` | 8 | 0.01 | The step of a value. |
| `axis` | `ideatek_slider_axis_t` | 12 | `IDEATEK_SLIDER_AXIS_HORIZONTAL` | The axis. |
| `disabled` | `bool` | 13 | `false` | `true` if it gives no event. |

Bytes 14 to 15 hold no field.

A step of 1 hundredth of the range is the default of Godot and of Unreal.

## ideatek:ideatek_scroll_flags_t

`ideatek_scroll_flags_t` names what scrolls a scroll area.
`ideatek_scroll_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SCROLL_FLAGS_WHEEL` | The wheel scrolls. |
| 0x02 | `IDEATEK_SCROLL_FLAGS_DRAG` | A drag scrolls. |

## ideatek:UiScrollArea

`ideatek:UiScrollArea` makes a node with an overflow of SCROLL scroll by the wheel or a drag.
`ideatek_ui_scroll_area_t` takes 2 bytes. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `flags` | `ideatek_scroll_flags_t` | 0 | WHEEL | What scrolls it. |
| `disabled` | `bool` | 1 | `false` | `true` if it does not scroll. |

## ideatek:UiScrollTo

`ideatek:UiScrollTo` scrolls a scroll area. The game spawns it as an event.
`ideatek_ui_scroll_to_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The scroll area. |
| `x` | `float` | 8 | The new scroll on X. |
| `y` | `float` | 12 | The new scroll on Y. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:ideatek_text_field_flags_t

`ideatek_text_field_flags_t` names the options of a text field.
`ideatek_text_field_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_TEXT_FIELD_FLAGS_SECRET` | The field hides its characters. |
| 0x02 | `IDEATEK_TEXT_FIELD_FLAGS_MULTILINE` | The field takes new lines. |

## ideatek:UiTextField

`ideatek:UiTextField` makes a text node a field that the player edits.
`ideatek_ui_text_field_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `max_length` | `uint32_t` | 0 | 0 | The most characters. 0 means no limit. |
| `flags` | `ideatek_text_field_flags_t` | 4 | 0 | The options. |
| `disabled` | `bool` | 5 | `false` | `true` if it gives no event. |

Bytes 6 to 7 hold no field.

## ideatek:UiTextChange

`ideatek:UiTextChange` proposes a new string of a text field.
`ideatek_ui_text_change_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The text field. |
| `text` | `bridge_text_t` | 8 | The proposed string. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:UiCaret

`ideatek:UiCaret` is the caret of a text field. The UI module writes it.
`ideatek_ui_caret_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `caret` | `uint32_t` | 0 | The place of the caret, in code points. |
| `anchor` | `uint32_t` | 4 | The other end of the selection. |

## ideatek:ideatek_screen_flags_t

`ideatek_screen_flags_t` names the options of a screen.
`ideatek_screen_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SCREEN_FLAGS_MODAL` | The screen blocks the roots below it. |
| 0x02 | `IDEATEK_SCREEN_FLAGS_RESTORE_FOCUS` | The focus comes back to the last node. |

## ideatek:UiScreen

`ideatek:UiScreen` makes a root a screen of a stack: *The screens*.
`ideatek_ui_screen_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `focus` | `ideatek_entity_t` | 0 | No default | The focus of the stack. |
| `flags` | `ideatek_screen_flags_t` | 8 | 0 | The options. |

Bytes 9 to 15 hold no field.

## ideatek:UiScreenState

`ideatek:UiScreenState` is the state of a screen. The UI module writes it.
`ideatek_ui_screen_state_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `last` | `ideatek_entity_t` | 0 | The last node with the focus. |
| `top` | `bool` | 8 | `true` if the screen is on top. |

Bytes 9 to 15 hold no field.

## ideatek:ideatek_screen_event_kind_t

`ideatek_screen_event_kind_t` tells the kind of a screen event.
`ideatek_screen_event_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SCREEN_EVENT_KIND_TOP` | The screen came on top. |
| 1 | `IDEATEK_SCREEN_EVENT_KIND_COVERED` | Another screen covered it. |

## ideatek:UiScreenEvent

`ideatek:UiScreenEvent` tells that a screen came on top or was covered.
`ideatek_ui_screen_event_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `screen` | `ideatek_entity_t` | 0 | The screen. |
| `order` | `uint32_t` | 8 | The rank in the round. |
| `kind` | `ideatek_screen_event_kind_t` | 12 | The kind. |

Bytes 13 to 15 hold no field.

## ideatek:ideatek_gesture_kind_t

`ideatek_gesture_kind_t` tells the kind of a gesture.
`ideatek_gesture_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GESTURE_KIND_TAP` | A short touch. |
| 1 | `IDEATEK_GESTURE_KIND_LONG_PRESS` | A long touch. |
| 2 | `IDEATEK_GESTURE_KIND_PAN` | A move of the fingers. |
| 3 | `IDEATEK_GESTURE_KIND_PINCH` | A pinch and a turn of 2 fingers. |

## ideatek:ideatek_gesture_phase_t

`ideatek_gesture_phase_t` tells the phase of a gesture.
`ideatek_gesture_phase_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GESTURE_PHASE_BEGIN` | The gesture starts. |
| 1 | `IDEATEK_GESTURE_PHASE_CHANGE` | The gesture goes on. |
| 2 | `IDEATEK_GESTURE_PHASE_END` | The gesture ends. |

## ideatek:UiGesture

`ideatek:UiGesture` tells a gesture of fingers. It is an event of 1 round.
`ideatek_ui_gesture_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `node` | `ideatek_entity_t` | 0 | The node under the start. 0 means the world. |
| `target` | `ideatek_entity_t` | 8 | The window or the texture. |
| `x` | `float` | 16 | The center of the fingers on X, in pixels. |
| `y` | `float` | 20 | The center on Y. |
| `dx` | `float` | 24 | The move on X since the event before. |
| `dy` | `float` | 28 | The move on Y since the event before. |
| `scale` | `float` | 32 | The distance of the fingers divided by the distance at the start. |
| `angle` | `float` | 36 | The turn since the start. |
| `vx` | `float` | 40 | The speed on X at the end, in pixels / s. |
| `vy` | `float` | 44 | The speed on Y at the end. |
| `order` | `uint32_t` | 48 | The rank in the round. |
| `kind` | `ideatek_gesture_kind_t` | 52 | The kind. |
| `phase` | `ideatek_gesture_phase_t` | 53 | The phase. |
| `fingers` | `uint8_t` | 54 | The number of fingers. |

Byte 55 holds no field.

## ideatek:ideatek_anchor_flags_t

`ideatek_anchor_flags_t` names the options of a marker.
`ideatek_anchor_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_ANCHOR_FLAGS_HIDE_BEHIND` | A point behind the camera hides the node. |

## ideatek:UiAnchor

`ideatek:UiAnchor` makes a node a marker of an entity of the world: *The markers*.
`ideatek_ui_anchor_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `entity` | `ideatek_entity_t` | 0 | No default | The entity of the world. |
| `camera` | `ideatek_entity_t` | 8 | No default | The camera. |
| `offset` | `ideatek_vec3_t` | 16 | No default | The offset of the point, in the world. |
| `pivot_x` | `float` | 28 | 0.5 | The point of the node on X, from 0 to 1. |
| `pivot_y` | `float` | 32 | 0.5 | The point of the node on Y, from 0 to 1. |
| `flags` | `ideatek_anchor_flags_t` | 36 | HIDE_BEHIND | The options. |

Bytes 37 to 39 hold no field.

A pivot of 0.5 is the default of the widget component of Unreal and of `transform-origin` of CSS.

## ideatek:UiAnchorState

`ideatek:UiAnchorState` is the state of a marker. The layout module writes it.
`ideatek_ui_anchor_state_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `distance` | `float` | 0 | The distance from the camera. |
| `visible` | `bool` | 4 | `true` if the node shows. |

Bytes 5 to 7 hold no field.

## ideatek:UiGrid

`ideatek:UiGrid` gives the cells of a grid node: *The grid*.
`ideatek_ui_grid_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `columns` | `uint32_t` | 0 | 1 | The columns. |
| `rows` | `uint32_t` | 4 | 0 | The rows. 0 adds rows as needed. |

## ideatek:UiGridItem

`ideatek:UiGridItem` places a child in a grid: *The grid*.
`ideatek_ui_grid_item_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `column` | `uint32_t` | 0 | 0 | The first column, from 1. 0 means the next free cell. |
| `row` | `uint32_t` | 4 | 0 | The first row, from 1. 0 means the next free cell. |
| `column_span` | `uint32_t` | 8 | 1 | The columns that it takes. |
| `row_span` | `uint32_t` | 12 | 1 | The rows that it takes. |

## ideatek:UiTransform

`ideatek:UiTransform` moves the draw of a node without the layout: *The transforms and the opacity*.
`ideatek_ui_transform_t` takes 40 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `x` | `float` | 0 | 0 | The move on X, in UI units. |
| `y` | `float` | 4 | 0 | The move on Y. |
| `scale_x` | `float` | 8 | 1 | The scale on X. |
| `scale_y` | `float` | 12 | 1 | The scale on Y. |
| `rotation` | `float` | 16 | 0 | The turn, in radians, clockwise. |
| `rotation_x` | `float` | 20 | 0 | The turn about X, in radians. |
| `rotation_y` | `float` | 24 | 0 | The turn about Y, in radians. |
| `perspective` | `float` | 28 | 0 | The distance of the eye, in UI units. 0 means none. |
| `pivot_x` | `float` | 32 | 0.5 | The center of the scale and the turn on X, from 0 to 1. |
| `pivot_y` | `float` | 36 | 0.5 | The center on Y, from 0 to 1. |

## ideatek:UiOpacity

`ideatek:UiOpacity` fades a node and its children: *The transforms and the opacity*.
`ideatek_ui_opacity_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `value` | `float` | 0 | 1 | The factor of the alpha, from 0 to 1. |

## ideatek:ideatek_gradient_kind_t

`ideatek_gradient_kind_t` tells the shape of a gradient.
`ideatek_gradient_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GRADIENT_KIND_LINEAR` | Along a line at `angle`. |
| 1 | `IDEATEK_GRADIENT_KIND_RADIAL` | Out from the center. |
| 2 | `IDEATEK_GRADIENT_KIND_CONIC` | About the center, from `angle`. |

## ideatek:UiGradient

`ideatek:UiGradient` draws a gradient over the background and under the image of a node.
`ideatek_ui_gradient_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `stops` | `bridge_text_t` | 0 | No default | The records of the stops. |
| `count` | `uint32_t` | 8 | No default | The records in `stops`. |
| `angle` | `float` | 12 | pi | The direction, in radians. 0 points up, and the angle turns clockwise. |
| `kind` | `ideatek_gradient_kind_t` | 16 | `IDEATEK_GRADIENT_KIND_LINEAR` | The shape. |

Bytes 17 to 19 hold no field.

A record of `stops` takes 20 bytes: `offset` (`float`, from 0 to 1) at 0, and `color` (`float[4]`, linear with alpha) at 4.
The records have no bytes between them.
The angle of pi points down, the default of `linear-gradient` of CSS.

## ideatek:UiAccessible

`ideatek:UiAccessible` names a node for a screen reader: *The accessibility*.
`ideatek_ui_accessible_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `label` | `bridge_text_t` | 0 | The name that a screen reader says. |
| `description` | `bridge_text_t` | 8 | More words about the node. |

## ideatek:UiShader

`ideatek:UiShader` draws the box of a node with a shader: *The shaders and the blur*.
`ideatek_ui_shader_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name of a shader of the render module. |
| `params` | `bridge_text_t` | 8 | The bytes that the shader reads. |

## ideatek:UiBlur

`ideatek:UiBlur` blurs a node, or what is behind it: *The shaders and the blur*.
`ideatek_ui_blur_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `radius` | `float` | 0 | No default | The radius of the blur, in UI units. |
| `backdrop` | `bool` | 4 | `false` | `true` if it blurs what is behind the box. |

Bytes 5 to 7 hold no field.

## ideatek:UiMask

`ideatek:UiMask` clips the children of a node by a shape: *The looks*.
`ideatek_ui_mask_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | 0 | The texture whose alpha clips the children. 0 means the box of the node. |

## ideatek:ideatek_ease_t

`ideatek_ease_t` tells the curve of a transition, as the tweens of RmlUi.
`ideatek_ease_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_EASE_LINEAR` | A straight blend. |
| 1 | `IDEATEK_EASE_QUADRATIC` | A curve of power 2. |
| 2 | `IDEATEK_EASE_CUBIC` | A curve of power 3. |
| 3 | `IDEATEK_EASE_QUARTIC` | A curve of power 4. |
| 4 | `IDEATEK_EASE_QUINTIC` | A curve of power 5. |
| 5 | `IDEATEK_EASE_SINE` | A sine curve. |
| 6 | `IDEATEK_EASE_EXPONENTIAL` | An exponential curve. |
| 7 | `IDEATEK_EASE_CIRCULAR` | A quarter of a circle. |
| 8 | `IDEATEK_EASE_BACK` | A curve that goes past the end and comes back. |
| 9 | `IDEATEK_EASE_ELASTIC` | A curve that springs. |
| 10 | `IDEATEK_EASE_BOUNCE` | A curve that bounces. |

## ideatek:ideatek_ease_direction_t

`ideatek_ease_direction_t` tells where the curve of a transition acts.
`ideatek_ease_direction_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_EASE_DIRECTION_IN` | The curve at the start. |
| 1 | `IDEATEK_EASE_DIRECTION_OUT` | The curve at the end. |
| 2 | `IDEATEK_EASE_DIRECTION_IN_OUT` | The curve at both ends. |

## ideatek:UiStyle

`ideatek:UiStyle` gives a node its looks for each state: *The styles*.
`ideatek_ui_style_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `normal` | `ideatek_entity_t` | 0 | No default | The style of the normal state. 0 means the node itself. |
| `hovered` | `ideatek_entity_t` | 8 | No default | The style of a hover. 0 means `normal`. |
| `pressed` | `ideatek_entity_t` | 16 | No default | The style of a press. 0 means `normal`. |
| `disabled` | `ideatek_entity_t` | 24 | No default | The style of a disabled widget. 0 means `normal`. |
| `focus` | `ideatek_entity_t` | 32 | No default | The looks over the state, with a visible focus. 0 means none. |
| `transition` | `float` | 40 | 0 | The seconds of the blend at a change of state. 0 means at once. |
| `ease` | `ideatek_ease_t` | 44 | `IDEATEK_EASE_LINEAR` | The curve of the blend. |
| `ease_direction` | `ideatek_ease_direction_t` | 45 | `IDEATEK_EASE_DIRECTION_OUT` | Where the curve acts. |

Bytes 46 to 47 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:UiRoot` | `ideatek_ui_root_t` | `ideatek_ui_root_t_desc` | `IDEATEK_UI_ROOT_FIELDS` |
| `ideatek:UiNode` | `ideatek_ui_node_t` | `ideatek_ui_node_t_desc` | `IDEATEK_UI_NODE_FIELDS` |
| `ideatek:ideatek_ui_unit_t` | `ideatek_ui_unit_t` | `ideatek_ui_unit_t_desc` | `IDEATEK_UI_UNIT_VALUES` |
| `ideatek:ideatek_ui_length_t` | `ideatek_ui_length_t` | `ideatek_ui_length_t_desc` | `IDEATEK_UI_LENGTH_FIELDS` |
| `ideatek:ideatek_ui_display_t` | `ideatek_ui_display_t` | `ideatek_ui_display_t_desc` | `IDEATEK_UI_DISPLAY_VALUES` |
| `ideatek:ideatek_ui_position_t` | `ideatek_ui_position_t` | `ideatek_ui_position_t_desc` | `IDEATEK_UI_POSITION_VALUES` |
| `ideatek:ideatek_ui_direction_t` | `ideatek_ui_direction_t` | `ideatek_ui_direction_t_desc` | `IDEATEK_UI_DIRECTION_VALUES` |
| `ideatek:ideatek_ui_align_t` | `ideatek_ui_align_t` | `ideatek_ui_align_t_desc` | `IDEATEK_UI_ALIGN_VALUES` |
| `ideatek:ideatek_ui_overflow_t` | `ideatek_ui_overflow_t` | `ideatek_ui_overflow_t_desc` | `IDEATEK_UI_OVERFLOW_VALUES` |
| `ideatek:UiLayout` | `ideatek_ui_layout_t` | `ideatek_ui_layout_t_desc` | `IDEATEK_UI_LAYOUT_FIELDS` |
| `ideatek:UiScroll` | `ideatek_ui_scroll_t` | `ideatek_ui_scroll_t_desc` | `IDEATEK_UI_SCROLL_FIELDS` |
| `ideatek:UiRect` | `ideatek_ui_rect_t` | `ideatek_ui_rect_t_desc` | `IDEATEK_UI_RECT_FIELDS` |
| `ideatek:ideatek_flip_t` | `ideatek_flip_t` | `ideatek_flip_t_desc` | `IDEATEK_FLIP_VALUES` |
| `ideatek:ideatek_image_mode_t` | `ideatek_image_mode_t` | `ideatek_image_mode_t_desc` | `IDEATEK_IMAGE_MODE_VALUES` |
| `ideatek:UiBox` | `ideatek_ui_box_t` | `ideatek_ui_box_t_desc` | `IDEATEK_UI_BOX_FIELDS` |
| `ideatek:UiImage` | `ideatek_ui_image_t` | `ideatek_ui_image_t_desc` | `IDEATEK_UI_IMAGE_FIELDS` |
| `ideatek:ideatek_font_format_t` | `ideatek_font_format_t` | `ideatek_font_format_t_desc` | `IDEATEK_FONT_FORMAT_VALUES` |
| `ideatek:Font` | `ideatek_font_t` | `ideatek_font_t_desc` | `IDEATEK_FONT_FIELDS` |
| `ideatek:UiText` | `ideatek_ui_text_t` | `ideatek_ui_text_t_desc` | `IDEATEK_UI_TEXT_FIELDS` |
| `ideatek:ideatek_text_align_t` | `ideatek_text_align_t` | `ideatek_text_align_t_desc` | `IDEATEK_TEXT_ALIGN_VALUES` |
| `ideatek:ideatek_text_wrap_t` | `ideatek_text_wrap_t` | `ideatek_text_wrap_t_desc` | `IDEATEK_TEXT_WRAP_VALUES` |
| `ideatek:ideatek_text_overflow_t` | `ideatek_text_overflow_t` | `ideatek_text_overflow_t_desc` | `IDEATEK_TEXT_OVERFLOW_VALUES` |
| `ideatek:UiTextLook` | `ideatek_ui_text_look_t` | `ideatek_ui_text_look_t_desc` | `IDEATEK_UI_TEXT_LOOK_FIELDS` |
| `ideatek:UiTextSpan` | `ideatek_ui_text_span_t` | `ideatek_ui_text_span_t_desc` | `IDEATEK_UI_TEXT_SPAN_FIELDS` |
| `ideatek:ideatek_pointer_buttons_t` | `ideatek_pointer_buttons_t` | `ideatek_pointer_buttons_t_desc` | `IDEATEK_POINTER_BUTTONS_VALUES` |
| `ideatek:UiPointer` | `ideatek_ui_pointer_t` | `ideatek_ui_pointer_t_desc` | `IDEATEK_UI_POINTER_FIELDS` |
| `ideatek:UiPointerHit` | `ideatek_ui_pointer_hit_t` | `ideatek_ui_pointer_hit_t_desc` | `IDEATEK_UI_POINTER_HIT_FIELDS` |
| `ideatek:ideatek_pickable_flags_t` | `ideatek_pickable_flags_t` | `ideatek_pickable_flags_t_desc` | `IDEATEK_PICKABLE_FLAGS_VALUES` |
| `ideatek:UiPickable` | `ideatek_ui_pickable_t` | `ideatek_ui_pickable_t_desc` | `IDEATEK_UI_PICKABLE_FIELDS` |
| `ideatek:ideatek_interaction_t` | `ideatek_interaction_t` | `ideatek_interaction_t_desc` | `IDEATEK_INTERACTION_VALUES` |
| `ideatek:UiInteraction` | `ideatek_ui_interaction_t` | `ideatek_ui_interaction_t_desc` | `IDEATEK_UI_INTERACTION_FIELDS` |
| `ideatek:ideatek_pointer_event_kind_t` | `ideatek_pointer_event_kind_t` | `ideatek_pointer_event_kind_t_desc` | `IDEATEK_POINTER_EVENT_KIND_VALUES` |
| `ideatek:UiPointerEvent` | `ideatek_ui_pointer_event_t` | `ideatek_ui_pointer_event_t_desc` | `IDEATEK_UI_POINTER_EVENT_FIELDS` |
| `ideatek:UiFocus` | `ideatek_ui_focus_t` | `ideatek_ui_focus_t_desc` | `IDEATEK_UI_FOCUS_FIELDS` |
| `ideatek:UiFocusState` | `ideatek_ui_focus_state_t` | `ideatek_ui_focus_state_t_desc` | `IDEATEK_UI_FOCUS_STATE_FIELDS` |
| `ideatek:ideatek_focusable_flags_t` | `ideatek_focusable_flags_t` | `ideatek_focusable_flags_t_desc` | `IDEATEK_FOCUSABLE_FLAGS_VALUES` |
| `ideatek:UiFocusable` | `ideatek_ui_focusable_t` | `ideatek_ui_focusable_t_desc` | `IDEATEK_UI_FOCUSABLE_FIELDS` |
| `ideatek:UiNeighbors` | `ideatek_ui_neighbors_t` | `ideatek_ui_neighbors_t_desc` | `IDEATEK_UI_NEIGHBORS_FIELDS` |
| `ideatek:UiFocusMove` | `ideatek_ui_focus_move_t` | `ideatek_ui_focus_move_t_desc` | `IDEATEK_UI_FOCUS_MOVE_FIELDS` |
| `ideatek:ideatek_focus_event_kind_t` | `ideatek_focus_event_kind_t` | `ideatek_focus_event_kind_t_desc` | `IDEATEK_FOCUS_EVENT_KIND_VALUES` |
| `ideatek:ideatek_focus_cause_t` | `ideatek_focus_cause_t` | `ideatek_focus_cause_t_desc` | `IDEATEK_FOCUS_CAUSE_VALUES` |
| `ideatek:UiFocusEvent` | `ideatek_ui_focus_event_t` | `ideatek_ui_focus_event_t_desc` | `IDEATEK_UI_FOCUS_EVENT_FIELDS` |
| `ideatek:UiValue` | `ideatek_ui_value_t` | `ideatek_ui_value_t_desc` | `IDEATEK_UI_VALUE_FIELDS` |
| `ideatek:UiValueEvent` | `ideatek_ui_value_event_t` | `ideatek_ui_value_event_t_desc` | `IDEATEK_UI_VALUE_EVENT_FIELDS` |
| `ideatek:ideatek_button_mode_t` | `ideatek_button_mode_t` | `ideatek_button_mode_t_desc` | `IDEATEK_BUTTON_MODE_VALUES` |
| `ideatek:UiButton` | `ideatek_ui_button_t` | `ideatek_ui_button_t_desc` | `IDEATEK_UI_BUTTON_FIELDS` |
| `ideatek:UiActivated` | `ideatek_ui_activated_t` | `ideatek_ui_activated_t_desc` | `IDEATEK_UI_ACTIVATED_FIELDS` |
| `ideatek:UiToggle` | `ideatek_ui_toggle_t` | `ideatek_ui_toggle_t_desc` | `IDEATEK_UI_TOGGLE_FIELDS` |
| `ideatek:UiToggleGroup` | `ideatek_ui_toggle_group_t` | `ideatek_ui_toggle_group_t_desc` | `IDEATEK_UI_TOGGLE_GROUP_FIELDS` |
| `ideatek:ideatek_slider_axis_t` | `ideatek_slider_axis_t` | `ideatek_slider_axis_t_desc` | `IDEATEK_SLIDER_AXIS_VALUES` |
| `ideatek:UiSlider` | `ideatek_ui_slider_t` | `ideatek_ui_slider_t_desc` | `IDEATEK_UI_SLIDER_FIELDS` |
| `ideatek:ideatek_scroll_flags_t` | `ideatek_scroll_flags_t` | `ideatek_scroll_flags_t_desc` | `IDEATEK_SCROLL_FLAGS_VALUES` |
| `ideatek:UiScrollArea` | `ideatek_ui_scroll_area_t` | `ideatek_ui_scroll_area_t_desc` | `IDEATEK_UI_SCROLL_AREA_FIELDS` |
| `ideatek:UiScrollTo` | `ideatek_ui_scroll_to_t` | `ideatek_ui_scroll_to_t_desc` | `IDEATEK_UI_SCROLL_TO_FIELDS` |
| `ideatek:ideatek_text_field_flags_t` | `ideatek_text_field_flags_t` | `ideatek_text_field_flags_t_desc` | `IDEATEK_TEXT_FIELD_FLAGS_VALUES` |
| `ideatek:UiTextField` | `ideatek_ui_text_field_t` | `ideatek_ui_text_field_t_desc` | `IDEATEK_UI_TEXT_FIELD_FIELDS` |
| `ideatek:UiTextChange` | `ideatek_ui_text_change_t` | `ideatek_ui_text_change_t_desc` | `IDEATEK_UI_TEXT_CHANGE_FIELDS` |
| `ideatek:UiCaret` | `ideatek_ui_caret_t` | `ideatek_ui_caret_t_desc` | `IDEATEK_UI_CARET_FIELDS` |
| `ideatek:ideatek_screen_flags_t` | `ideatek_screen_flags_t` | `ideatek_screen_flags_t_desc` | `IDEATEK_SCREEN_FLAGS_VALUES` |
| `ideatek:UiScreen` | `ideatek_ui_screen_t` | `ideatek_ui_screen_t_desc` | `IDEATEK_UI_SCREEN_FIELDS` |
| `ideatek:UiScreenState` | `ideatek_ui_screen_state_t` | `ideatek_ui_screen_state_t_desc` | `IDEATEK_UI_SCREEN_STATE_FIELDS` |
| `ideatek:ideatek_screen_event_kind_t` | `ideatek_screen_event_kind_t` | `ideatek_screen_event_kind_t_desc` | `IDEATEK_SCREEN_EVENT_KIND_VALUES` |
| `ideatek:UiScreenEvent` | `ideatek_ui_screen_event_t` | `ideatek_ui_screen_event_t_desc` | `IDEATEK_UI_SCREEN_EVENT_FIELDS` |
| `ideatek:ideatek_gesture_kind_t` | `ideatek_gesture_kind_t` | `ideatek_gesture_kind_t_desc` | `IDEATEK_GESTURE_KIND_VALUES` |
| `ideatek:ideatek_gesture_phase_t` | `ideatek_gesture_phase_t` | `ideatek_gesture_phase_t_desc` | `IDEATEK_GESTURE_PHASE_VALUES` |
| `ideatek:UiGesture` | `ideatek_ui_gesture_t` | `ideatek_ui_gesture_t_desc` | `IDEATEK_UI_GESTURE_FIELDS` |
| `ideatek:ideatek_anchor_flags_t` | `ideatek_anchor_flags_t` | `ideatek_anchor_flags_t_desc` | `IDEATEK_ANCHOR_FLAGS_VALUES` |
| `ideatek:UiAnchor` | `ideatek_ui_anchor_t` | `ideatek_ui_anchor_t_desc` | `IDEATEK_UI_ANCHOR_FIELDS` |
| `ideatek:UiAnchorState` | `ideatek_ui_anchor_state_t` | `ideatek_ui_anchor_state_t_desc` | `IDEATEK_UI_ANCHOR_STATE_FIELDS` |
| `ideatek:UiGrid` | `ideatek_ui_grid_t` | `ideatek_ui_grid_t_desc` | `IDEATEK_UI_GRID_FIELDS` |
| `ideatek:UiGridItem` | `ideatek_ui_grid_item_t` | `ideatek_ui_grid_item_t_desc` | `IDEATEK_UI_GRID_ITEM_FIELDS` |
| `ideatek:UiTransform` | `ideatek_ui_transform_t` | `ideatek_ui_transform_t_desc` | `IDEATEK_UI_TRANSFORM_FIELDS` |
| `ideatek:UiOpacity` | `ideatek_ui_opacity_t` | `ideatek_ui_opacity_t_desc` | `IDEATEK_UI_OPACITY_FIELDS` |
| `ideatek:ideatek_gradient_kind_t` | `ideatek_gradient_kind_t` | `ideatek_gradient_kind_t_desc` | `IDEATEK_GRADIENT_KIND_VALUES` |
| `ideatek:UiGradient` | `ideatek_ui_gradient_t` | `ideatek_ui_gradient_t_desc` | `IDEATEK_UI_GRADIENT_FIELDS` |
| `ideatek:UiAccessible` | `ideatek_ui_accessible_t` | `ideatek_ui_accessible_t_desc` | `IDEATEK_UI_ACCESSIBLE_FIELDS` |
| `ideatek:UiShader` | `ideatek_ui_shader_t` | `ideatek_ui_shader_t_desc` | `IDEATEK_UI_SHADER_FIELDS` |
| `ideatek:UiBlur` | `ideatek_ui_blur_t` | `ideatek_ui_blur_t_desc` | `IDEATEK_UI_BLUR_FIELDS` |
| `ideatek:UiMask` | `ideatek_ui_mask_t` | `ideatek_ui_mask_t_desc` | `IDEATEK_UI_MASK_FIELDS` |
| `ideatek:ideatek_ease_t` | `ideatek_ease_t` | `ideatek_ease_t_desc` | `IDEATEK_EASE_VALUES` |
| `ideatek:ideatek_ease_direction_t` | `ideatek_ease_direction_t` | `ideatek_ease_direction_t_desc` | `IDEATEK_EASE_DIRECTION_VALUES` |
| `ideatek:UiStyle` | `ideatek_ui_style_t` | `ideatek_ui_style_t_desc` | `IDEATEK_UI_STYLE_FIELDS` |

```text
{ ideatek_entity_t target; int32_t order; float scale; bool right_to_left; }
{ int32_t order; }
{ IDEATEK_UI_UNIT_AUTO = 0, IDEATEK_UI_UNIT_PX = 1, IDEATEK_UI_UNIT_PERCENT = 2 }
{ float value; ideatek_ui_unit_t unit; }
{ IDEATEK_UI_DISPLAY_NONE = 0, IDEATEK_UI_DISPLAY_FLEX = 1, IDEATEK_UI_DISPLAY_GRID = 2 }
{ IDEATEK_UI_POSITION_RELATIVE = 0, IDEATEK_UI_POSITION_ABSOLUTE = 1 }
{ IDEATEK_UI_DIRECTION_ROW = 0, IDEATEK_UI_DIRECTION_COLUMN = 1, IDEATEK_UI_DIRECTION_ROW_REVERSE = 2, IDEATEK_UI_DIRECTION_COLUMN_REVERSE = 3 }
{ IDEATEK_UI_ALIGN_START = 0, IDEATEK_UI_ALIGN_CENTER = 1, IDEATEK_UI_ALIGN_END = 2, IDEATEK_UI_ALIGN_STRETCH = 3, IDEATEK_UI_ALIGN_SPACE_BETWEEN = 4, IDEATEK_UI_ALIGN_SPACE_AROUND = 5, IDEATEK_UI_ALIGN_SPACE_EVENLY = 6 }
{ IDEATEK_UI_OVERFLOW_VISIBLE = 0, IDEATEK_UI_OVERFLOW_CLIP = 1, IDEATEK_UI_OVERFLOW_SCROLL = 2 }
{ ideatek_ui_length_t left; ideatek_ui_length_t right; ideatek_ui_length_t top; ideatek_ui_length_t bottom; ideatek_ui_length_t width; ideatek_ui_length_t height; ideatek_ui_length_t min_width; ideatek_ui_length_t min_height; ideatek_ui_length_t max_width; ideatek_ui_length_t max_height; ideatek_ui_length_t basis; float aspect; float margin_left; float margin_right; float margin_top; float margin_bottom; float padding_left; float padding_right; float padding_top; float padding_bottom; float border_left; float border_right; float border_top; float border_bottom; float grow; float shrink; float gap_x; float gap_y; ideatek_ui_display_t display; ideatek_ui_position_t position; ideatek_ui_direction_t direction; bool wrap; ideatek_ui_align_t align_items; ideatek_ui_align_t align_self; ideatek_ui_align_t justify_content; ideatek_ui_align_t align_content; ideatek_ui_overflow_t overflow_x; ideatek_ui_overflow_t overflow_y; }
{ float x; float y; }
{ float x; float y; float width; float height; float content_width; float content_height; float clip_x; float clip_y; float clip_width; float clip_height; }
{ IDEATEK_FLIP_X = 0x01, IDEATEK_FLIP_Y = 0x02 }
{ IDEATEK_IMAGE_MODE_STRETCH = 0, IDEATEK_IMAGE_MODE_SLICED = 1, IDEATEK_IMAGE_MODE_TILED = 2 }
{ ideatek_color_t background; ideatek_color_t border; ideatek_color_t shadow; float radius[4]; float shadow_x; float shadow_y; float shadow_blur; float shadow_spread; }
{ ideatek_entity_t texture; ideatek_color_t tint; float uv_x; float uv_y; float uv_width; float uv_height; float slice_left; float slice_top; float slice_right; float slice_bottom; ideatek_image_mode_t mode; ideatek_flip_t flip; }
{ IDEATEK_FONT_FORMAT_SFNT = 0 }
{ ideatek_entity_t fallback; bridge_text_t bytes; uint32_t revision; ideatek_font_format_t format; }
{ bridge_text_t text; }
{ IDEATEK_TEXT_ALIGN_START = 0, IDEATEK_TEXT_ALIGN_CENTER = 1, IDEATEK_TEXT_ALIGN_END = 2, IDEATEK_TEXT_ALIGN_JUSTIFY = 3 }
{ IDEATEK_TEXT_WRAP_WORD = 0, IDEATEK_TEXT_WRAP_ANY = 1, IDEATEK_TEXT_WRAP_NONE = 2 }
{ IDEATEK_TEXT_OVERFLOW_CLIP = 0, IDEATEK_TEXT_OVERFLOW_ELLIPSIS = 1 }
{ ideatek_entity_t font; ideatek_color_t color; ideatek_color_t outline_color; ideatek_color_t shadow; float size; float line_height; float outline; float shadow_x; float shadow_y; float letter_spacing; ideatek_text_align_t align; ideatek_text_wrap_t wrap; ideatek_text_overflow_t overflow; }
{ bridge_text_t text; ideatek_entity_t font; ideatek_entity_t image; ideatek_color_t color; float size; }
{ IDEATEK_POINTER_BUTTONS_PRIMARY = 0x01, IDEATEK_POINTER_BUTTONS_SECONDARY = 0x02, IDEATEK_POINTER_BUTTONS_MIDDLE = 0x04 }
{ ideatek_entity_t target; float x; float y; float wheel_x; float wheel_y; ideatek_pointer_buttons_t buttons; }
{ ideatek_entity_t node; }
{ IDEATEK_PICKABLE_FLAGS_BLOCK = 0x01, IDEATEK_PICKABLE_FLAGS_HOVER = 0x02 }
{ ideatek_pickable_flags_t flags; }
{ IDEATEK_INTERACTION_NONE = 0, IDEATEK_INTERACTION_HOVERED = 1, IDEATEK_INTERACTION_PRESSED = 2 }
{ ideatek_entity_t pointer; ideatek_interaction_t state; }
{ IDEATEK_POINTER_EVENT_KIND_ENTER = 0, IDEATEK_POINTER_EVENT_KIND_LEAVE = 1, IDEATEK_POINTER_EVENT_KIND_PRESS = 2, IDEATEK_POINTER_EVENT_KIND_RELEASE = 3, IDEATEK_POINTER_EVENT_KIND_CLICK = 4, IDEATEK_POINTER_EVENT_KIND_DRAG_START = 5, IDEATEK_POINTER_EVENT_KIND_DRAG = 6, IDEATEK_POINTER_EVENT_KIND_DRAG_END = 7, IDEATEK_POINTER_EVENT_KIND_DROP = 8, IDEATEK_POINTER_EVENT_KIND_SCROLL = 9 }
{ ideatek_entity_t node; ideatek_entity_t pointer; ideatek_entity_t other; float x; float y; float dx; float dy; uint32_t order; ideatek_pointer_event_kind_t kind; ideatek_pointer_buttons_t button; }
{ ideatek_entity_t root; ideatek_entity_t up; ideatek_entity_t down; ideatek_entity_t left; ideatek_entity_t right; ideatek_entity_t next; ideatek_entity_t previous; ideatek_entity_t accept; ideatek_entity_t cancel; }
{ ideatek_entity_t node; bool visible; }
{ IDEATEK_FOCUSABLE_FLAGS_CLICK = 0x01, IDEATEK_FOCUSABLE_FLAGS_NAVIGATE = 0x02, IDEATEK_FOCUSABLE_FLAGS_AUTO = 0x04 }
{ int32_t tab; ideatek_focusable_flags_t flags; }
{ ideatek_entity_t up; ideatek_entity_t down; ideatek_entity_t left; ideatek_entity_t right; }
{ ideatek_entity_t focus; ideatek_entity_t node; uint32_t order; }
{ IDEATEK_FOCUS_EVENT_KIND_GAINED = 0, IDEATEK_FOCUS_EVENT_KIND_LOST = 1, IDEATEK_FOCUS_EVENT_KIND_ACTIVATE = 2, IDEATEK_FOCUS_EVENT_KIND_CANCEL = 3 }
{ IDEATEK_FOCUS_CAUSE_NAVIGATE = 0, IDEATEK_FOCUS_CAUSE_POINTER = 1, IDEATEK_FOCUS_CAUSE_AUTO = 2, IDEATEK_FOCUS_CAUSE_GAME = 3 }
{ ideatek_entity_t focus; ideatek_entity_t node; uint32_t order; ideatek_focus_event_kind_t kind; ideatek_focus_cause_t cause; }
{ float value; }
{ ideatek_entity_t node; float value; uint32_t order; }
{ IDEATEK_BUTTON_MODE_RELEASE = 0, IDEATEK_BUTTON_MODE_PRESS = 1 }
{ ideatek_button_mode_t mode; bool disabled; }
{ ideatek_entity_t node; ideatek_entity_t source; uint32_t order; }
{ ideatek_entity_t group; bool disabled; }
{ bool allow_none; }
{ IDEATEK_SLIDER_AXIS_HORIZONTAL = 0, IDEATEK_SLIDER_AXIS_VERTICAL = 1 }
{ float min; float max; float step; ideatek_slider_axis_t axis; bool disabled; }
{ IDEATEK_SCROLL_FLAGS_WHEEL = 0x01, IDEATEK_SCROLL_FLAGS_DRAG = 0x02 }
{ ideatek_scroll_flags_t flags; bool disabled; }
{ ideatek_entity_t node; float x; float y; uint32_t order; }
{ IDEATEK_TEXT_FIELD_FLAGS_SECRET = 0x01, IDEATEK_TEXT_FIELD_FLAGS_MULTILINE = 0x02 }
{ uint32_t max_length; ideatek_text_field_flags_t flags; bool disabled; }
{ ideatek_entity_t node; bridge_text_t text; uint32_t order; }
{ uint32_t caret; uint32_t anchor; }
{ IDEATEK_SCREEN_FLAGS_MODAL = 0x01, IDEATEK_SCREEN_FLAGS_RESTORE_FOCUS = 0x02 }
{ ideatek_entity_t focus; ideatek_screen_flags_t flags; }
{ ideatek_entity_t last; bool top; }
{ IDEATEK_SCREEN_EVENT_KIND_TOP = 0, IDEATEK_SCREEN_EVENT_KIND_COVERED = 1 }
{ ideatek_entity_t screen; uint32_t order; ideatek_screen_event_kind_t kind; }
{ IDEATEK_GESTURE_KIND_TAP = 0, IDEATEK_GESTURE_KIND_LONG_PRESS = 1, IDEATEK_GESTURE_KIND_PAN = 2, IDEATEK_GESTURE_KIND_PINCH = 3 }
{ IDEATEK_GESTURE_PHASE_BEGIN = 0, IDEATEK_GESTURE_PHASE_CHANGE = 1, IDEATEK_GESTURE_PHASE_END = 2 }
{ ideatek_entity_t node; ideatek_entity_t target; float x; float y; float dx; float dy; float scale; float angle; float vx; float vy; uint32_t order; ideatek_gesture_kind_t kind; ideatek_gesture_phase_t phase; uint8_t fingers; }
{ IDEATEK_ANCHOR_FLAGS_HIDE_BEHIND = 0x01 }
{ ideatek_entity_t entity; ideatek_entity_t camera; ideatek_vec3_t offset; float pivot_x; float pivot_y; ideatek_anchor_flags_t flags; }
{ float distance; bool visible; }
{ uint32_t columns; uint32_t rows; }
{ uint32_t column; uint32_t row; uint32_t column_span; uint32_t row_span; }
{ float x; float y; float scale_x; float scale_y; float rotation; float rotation_x; float rotation_y; float perspective; float pivot_x; float pivot_y; }
{ float value; }
{ IDEATEK_GRADIENT_KIND_LINEAR = 0, IDEATEK_GRADIENT_KIND_RADIAL = 1, IDEATEK_GRADIENT_KIND_CONIC = 2 }
{ bridge_text_t stops; uint32_t count; float angle; ideatek_gradient_kind_t kind; }
{ bridge_text_t label; bridge_text_t description; }
{ bridge_text_t name; bridge_text_t params; }
{ float radius; bool backdrop; }
{ ideatek_entity_t texture; }
{ IDEATEK_EASE_LINEAR = 0, IDEATEK_EASE_QUADRATIC = 1, IDEATEK_EASE_CUBIC = 2, IDEATEK_EASE_QUARTIC = 3, IDEATEK_EASE_QUINTIC = 4, IDEATEK_EASE_SINE = 5, IDEATEK_EASE_EXPONENTIAL = 6, IDEATEK_EASE_CIRCULAR = 7, IDEATEK_EASE_BACK = 8, IDEATEK_EASE_ELASTIC = 9, IDEATEK_EASE_BOUNCE = 10 }
{ IDEATEK_EASE_DIRECTION_IN = 0, IDEATEK_EASE_DIRECTION_OUT = 1, IDEATEK_EASE_DIRECTION_IN_OUT = 2 }
{ ideatek_entity_t normal; ideatek_entity_t hovered; ideatek_entity_t pressed; ideatek_entity_t disabled; ideatek_entity_t focus; float transition; ideatek_ease_t ease; ideatek_ease_direction_t ease_direction; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_color_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_unit_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_length_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_display_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_position_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_direction_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_align_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ui_overflow_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_flip_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_image_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_font_format_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_align_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_wrap_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_overflow_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pointer_buttons_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pickable_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_interaction_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pointer_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_focusable_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_focus_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_focus_cause_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_button_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_slider_axis_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_scroll_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_text_field_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_screen_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_screen_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gesture_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gesture_phase_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_anchor_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gradient_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ease_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ease_direction_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiRoot` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiNode` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiLayout` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiScroll` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiRect` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiBox` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiImage` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Font` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiText` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiTextLook` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiTextSpan` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiPointer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiPointerHit` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiPickable` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiInteraction` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiPointerEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiFocus` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiFocusState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiFocusable` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiNeighbors` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiFocusMove` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiFocusEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiValue` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiValueEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiButton` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiActivated` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiToggle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiToggleGroup` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiSlider` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiScrollArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiScrollTo` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiTextField` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiTextChange` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiCaret` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiScreen` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiScreenState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiScreenEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiGesture` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiAnchor` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiAnchorState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiGrid` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiGridItem` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiTransform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiOpacity` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiGradient` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiAccessible` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiShader` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiBlur` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiMask` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:UiStyle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The roots, the nodes, the looks, the texts, the fonts, the focus, the widgets, the screens, the markers, the styles and the labels | The types of the fields |
| The UI module | The rectangles, the scrolls, the pointers, the hits, the states, the events and the carets | The types of the game, `ideatek:WorldTransform`, `ideatek:AnimationCurves`, `ideatek:LocalizedText`, and the types of the fields |
