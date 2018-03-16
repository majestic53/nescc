# Nescc example code
# ---
# Attribution:
#   Code ported from Erik Onarheim's project (https://github.com/eonarheim/nesgame, https://www.youtube.com/watch?v=mnUXYl5B9Qs)

# ------------------------------------------------------------------------------------
# iNes header
# ------------------------------------------------------------------------------------
.inesprg 1
.ineschr 1
.inesmap 0
.inesmir 1

# ------------------------------------------------------------------------------------
# Zero page variables
# ------------------------------------------------------------------------------------
.prg 0
.org 0x0000

loop_count: .rs 1
player_x: .rs 1
player_xv: .rs 1
player_y: .rs 1
player_yv: .rs 1
controller: .rs 1
gravity: .rs 1
ground: .rs 1
in_air: .rs 1
enemy_x: .rs 1
enemy_y: .rs 1
background_low: .rs 1
background_high: .rs 1
counter_low: .rs 1
counter_high: .rs 1
scroll: .rs 1
nametable: .rs 1
column_low: .rs 1
column_high: .rs 1
source_low: .rs 1
source_high: .rs 1
column_number: .rs 1

# ------------------------------------------------------------------------------------
# NES reset
# ------------------------------------------------------------------------------------
.org 0xc000

reset:
	sei
	cld
	ldx @0x40
	stx 0x4017
	ldx @0xff
	txs
	inx
	stx 0x2000
	stx 0x2001
	stx 0x4010

wait_vblank_0:
	bit 0x2002
	bpl wait_vblank_0

clear_memory:
	lda @0x00
	sta 0x0000, x
	sta 0x0100, x
	sta 0x0300, x
	sta 0x0400, x
	sta 0x0500, x
	sta 0x0600, x
	sta 0x0700, x
	lda @0xfe
	sta 0x0200, x
	inx
	bne clear_memory

wait_vblank_1:
	bit 0x2002
	bpl wait_vblank_1

# ------------------------------------------------------------------------------------
# Load game elements
# ------------------------------------------------------------------------------------
load_palettes:
	lda 0x2002
	lda @0x3f
	sta 0x2006
	lda @0x00
	sta 0x2006
	ldx @0x00
load_palettes_loop:
	lda palette, x
	sta 0x2007
	inx
	cpx @0x20
	bne load_palettes_loop

load_sprites:
	ldx @0x00
load_sprites_loop:
	lda player_sprite, x
	sta 0x0200, x
	inx
	cpx @0x1c
	bne load_sprites_loop

load_background:
	lda 0x2002
	lda @0x20
	sta 0x2006
	lda @0x00
	sta 0x2006
	ldx @0x00
	lda @0x5E #.low background
	sta background_low
	lda @0xE0 #.high background
	sta background_high
	lda @0xc0
	sta counter_low
	lda @0x03
	sta counter_high
	ldy @0x00
load_background_loop:
	lda [background_low], y
	sta 0x2007
	lda background_low
	clc
	adc @0x01
	sta background_low
	lda background_high
	adc @0x00
	sta background_high
	lda counter_low
	sec
	sbc @0x01
	sta counter_low
	lda counter_high
	sbc @0x00
	sta counter_high
	lda counter_low
	cmp @0x00
	bne load_background_loop
	lda counter_high
	cmp @0x00
	bne load_background_loop

load_attribute:
	lda 0x2002
	lda @0x23
	sta 0x2006
	lda @0xc0
	sta 0x2006
	ldx @0x00
load_attribute_loop:
	lda attribute, x
	sta 0x2007
	inx
	cpx @0x40
	bne load_attribute_loop

# ------------------------------------------------------------------------------------
# Initialize PPU
# ------------------------------------------------------------------------------------
initialize_ppu:
	lda @0b10010000
	sta 0x2000
	lda @0b00011110
	sta 0x2001

# ------------------------------------------------------------------------------------
# Initialze game state
# ------------------------------------------------------------------------------------
initialze_state:
	lda @0x80
	sta player_x
	lda @0x00
	sta player_y
	lda @0xc8
	sta ground
	lda @0x00
	sta controller
	sta player_xv
	sta player_yv
	sta loop_count
	lda @0x03
	sta gravity

forever:
	jmp forever

# ------------------------------------------------------------------------------------
# Game loop
# ------------------------------------------------------------------------------------
mainLoop:
	lda @0x00
	sta 0x2003
	lda @0x02
	sta 0x4014
	jsr render
	jsr update
	rti

# ------------------------------------------------------------------------------------
# Render game elements
# ------------------------------------------------------------------------------------
render:
	jsr render_player
	lda @0b10010000
	sta 0x2000
	lda @0b00011110
	sta 0x2001
	lda @0x00
	sta 0x2005
	sta 0x2005
	rts

render_player:
	ldx @0x00
	ldy @0x00
render_player_loop:
	lda 0x0203, x
	clc
	lda player_sprite_offset, y
	adc player_x
	iny
	sta 0x0203, x
	lda 0x0200, x
	clc
	lda player_sprite_offset, y
	adc player_y
	iny
	sta 0x0200, x
	inx
	inx
	inx
	inx
	cpx @0x1c
	bne render_player_loop
	rts

render_enemy:
	ldx @0x00
	ldy @0x00
render_enemy_loop:
	lda 0x0203, x
	clc
	lda enemy_sprite_offset, y
	adc enemy_x
	iny
	sta 0x0203, x
	lda 0x0200, x
	clc
	lda player_sprite_offset, y
	adc enemy_y
	iny
	sta 0x0200, x
	inx
	inx
	inx
	inx
	cpx @0x08
	bne render_enemy_loop
	rts

# ------------------------------------------------------------------------------------
# Update game elements
# ------------------------------------------------------------------------------------
update:
	jsr latch_controller
	jsr poll_controller
	jsr read_left
	jsr read_right
	jsr read_a
	inc loop_count
	jsr update_player_position
	rts

update_player_position:
	clc
	lda loop_count
	cmp @0x0a
	bne skip_gravity
	lda gravity
	adc player_yv
	sta player_yv
	lda @0x00
	sta loop_count
skip_gravity:
	lda player_yv
	clc
	adc player_y
	sta player_y
	cmp ground
	bcs put_player_on_ground
player_on_ground_done:
	lda player_xv
	clc
	adc player_x
	sta player_x
	rts
put_player_on_ground:
	lda ground
	sta player_y
	lda @0x00
	sta player_yv
	sta in_air
	jmp player_on_ground_done

# ------------------------------------------------------------------------------------
# Read controller
# ------------------------------------------------------------------------------------
latch_controller:
	lda @0x01
	sta 0x4016
	lda @0x00
	sta 0x4016
	rts

poll_controller:
	ldx @0x00
poll_controller_loop:
	lda 0x4016
	lsr a
	rol controller
	inx
	cpx @0x08
	bne poll_controller_loop
	rts

read_right:
	lda controller
	and @0b00000001
	beq read_right_done
	lda 0x0203
	clc
	lda player_x
	adc @0x02
	sta player_x
read_right_done:
	rts

read_left:
	lda controller
	and @0b00000010
	beq read_left_done
	lda 0x0203
	clc
	lda player_x
	adc @0xfe
	sta player_x
read_left_done:
	RTS

read_a:
	lda controller
	and @0b10000000
	beq read_a_done
	lda in_air
	cmp @0x01
	beq read_a_done
	lda ground
	sta player_y
	lda @0xfa
	sta player_yv
	lda @0x01
	sta in_air
read_a_done:
	rts

# ------------------------------------------------------------------------------------
# game data
# ------------------------------------------------------------------------------------
.org 0xe000

palette:
	.db 0x08, 0x1a, 0x38, 0x18, 0x08, 0x02, 0x38, 0x3c, 0x08, 0x1c, 0x15, 0x14, 0x08, 0x02, 0x38, 0x2a
	.db 0x21, 0x2c, 0x11, 0x15, 0x0f, 0x35, 0x36, 0x37, 0x0f, 0x39, 0x3a, 0x3b, 0x0f, 0x3d, 0x3e, 0x0f

player_sprite:
	.db 0x80, 0x00, 0x00, 0x80
	.db 0x80, 0x01, 0x00, 0x88
	.db 0x80, 0x02, 0x00, 0x90
	.db 0x88, 0x10, 0x00, 0x80
	.db 0x88, 0x11, 0x00, 0x88
	.db 0x88, 0x12, 0x00, 0x90
	.db 0x90, 0x21, 0x00, 0x88

player_sprite_offset:
	.db 0xf8, 0xf0
	.db 0x00, 0xf0
	.db 0x08, 0xf0
	.db 0xf8, 0xf8
	.db 0x00, 0xf8
	.db 0x08, 0xf8
	.db 0x00, 0x00

enemy_sprite_frame_0:
	.db 0xf0, 0x04, 0x00, 0x00
	.db 0xf0, 0x05, 0x00, 0x08

enemy_sprite_frame_1:
	.db 0xf0, 0x14, 0x00, 0x00
	.db 0xf0, 0x15, 0x00, 0x08

enemy_sprite_offset:
	.db 0x00, 0x00
	.db 0x00, 0x08

background:
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
	.db 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01
	.db 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01
	.db 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.db 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.db 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.db 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.db 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
	.db 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10

attribute:
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00
	.db 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00

# ------------------------------------------------------------------------------------
# Interrupt vectors
# ------------------------------------------------------------------------------------
.org 0xfffa

nescallback:
	.dw mainLoop
	.dw reset
	.dw 0x0000

.chr 0
.org 0x0000

.inc "example.chr"