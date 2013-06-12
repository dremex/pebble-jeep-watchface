#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID { 0x20, 0xB4, 0x45, 0x40, 0x82, 0x54, 0x4D, 0x5F, 0x8F, 0xA3, 0x27, 0x17, 0xC9, 0xE2, 0x48, 0x16 }
PBL_APP_INFO(MY_UUID,
			"Jeep", "Kerry Peterson",
			1, 0, /* App version */
			RESOURCE_ID_MENU_ICON_JEEP,
			APP_INFO_WATCH_FACE);

Window window;
TextLayer text_time_layer;
TextLayer text_date_layer;
BmpContainer background_image;

void handle_init(AppContextRef ctx) {
	(void)ctx;

	window_init(&window, "Jeep");
	window_stack_push(&window, true /* Animated */);

	resource_init_current_app(&APP_RESOURCES);

	bmp_init_container(RESOURCE_ID_JEEP_BACKGROUND, &background_image);
	layer_add_child(&window.layer, &background_image.layer.layer);

	text_layer_init(&text_time_layer, window.layer.frame);
	text_layer_set_text_color(&text_time_layer, GColorBlack);
	text_layer_set_background_color(&text_time_layer, GColorClear);
	layer_set_frame(&text_time_layer.layer, GRect(0, 10, 144, 50));
	text_layer_set_font(&text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HELVETICA_BLACK_44)));
	layer_add_child(&window.layer, &text_time_layer.layer);

	text_layer_init(&text_date_layer, window.layer.frame);
	text_layer_set_text_color(&text_date_layer, GColorBlack);
	text_layer_set_background_color(&text_date_layer, GColorClear);
	layer_set_frame(&text_date_layer.layer, GRect(0, 55, 144, 50));
	text_layer_set_font(&text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HELVETICA_BLACK_26)));
	layer_add_child(&window.layer, &text_date_layer.layer);
}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
	(void)ctx;

	static char time_text[] = "00:00";
	static char date_text[] = "Mon 10";

	char *time_format;

	if (clock_is_24h_style()) {
		time_format = "%R";
	} else {
		time_format = "%I:%M";
	}

	string_format_time(time_text, sizeof(time_text), time_format, t->tick_time);
	string_format_time(date_text, sizeof(date_text), "%a %d", t->tick_time);

	if (!clock_is_24h_style() && (time_text[0] == '0')) {
		memmove(time_text, &time_text[1], sizeof(time_text) - 1);
	}

	text_layer_set_text(&text_time_layer, time_text);
	text_layer_set_text(&text_date_layer, date_text);

	text_layer_set_text_alignment(&text_time_layer, GTextAlignmentCenter);
	text_layer_set_text_alignment(&text_date_layer, GTextAlignmentCenter);
}


void pbl_main(void *params) {
	PebbleAppHandlers handlers = {
		.init_handler = &handle_init,

		.tick_info = {
			.tick_handler = &handle_minute_tick,
			.tick_units = MINUTE_UNIT
		}
	};
	app_event_loop(params, &handlers);
}
