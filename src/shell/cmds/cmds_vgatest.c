#include "cmds_vgatest.h"
#include "../../klibc/libc.h"

#define LOREM_IPSUM                                                            \
  "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. "   \
  "Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies "  \
  "sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius "   \
  "a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy "      \
  "molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. "    \
  "Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, " \
  "enim. Pellentesque congue. Ut in risus volutpat libero pharetra tempor. "   \
  "Cras vestibulum bibendum augue. Praesent egestas leo in pede. Praesent "    \
  "blandit odio eu enim. Pellentesque sed dui ut augue blandit sodales. "      \
  "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere "  \
  "cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque "       \
  "fermentum. Maecenas adipiscing ante non diam sodales hendrerit.\n\nUt "     \
  "velit mauris, egestas sed, gravida nec, ornare ut, mi. Aenean ut orci vel " \
  "massa suscipit pulvinar. Nulla sollicitudin. Fusce varius, ligula non "     \
  "tempus aliquam, nunc turpis ullamcorper nibh, in tempus sapien eros vitae " \
  "ligula. Pellentesque rhoncus nunc et augue. Integer id felis. Curabitur "   \
  "aliquet pellentesque diam. Integer quis metus vitae elit lobortis "         \
  "egestas. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Morbi "  \
  "vel erat non mauris convallis vehicula. Nulla et sapien. Integer tortor "   \
  "tellus, aliquam faucibus, convallis id, congue eu, quam. Mauris "           \
  "ullamcorper felis vitae erat. Proin feugiat, augue non elementum posuere, " \
  "metus purus iaculis lectus, et tristique ligula justo vitae "               \
  "magna.\nAliquam convallis sollicitudin purus. Praesent aliquam, enim at "   \
  "fermentum mollis, ligula massa adipiscing nisl, ac euismod nibh nisl eu "   \
  "lectus. Fusce vulputate sem at sapien. Vivamus leo. Aliquam euismod "       \
  "libero eu enim. Nulla nec felis sed leo placerat imperdiet. Aenean "        \
  "suscipit nulla in justo. Suspendisse cursus rutrum augue. Nulla tincidunt " \
  "tincidunt mi. Curabitur iaculis, lorem vel rhoncus faucibus, felis magna "  \
  "fermentum augue, et ultricies lacus lorem varius purus. Curabitur eu "      \
  "amet.\n"

/// @brief create a light box on a given screen
/// @param screen_nbr nbr of the screen to write to
/// @param width width of the box, starting from 0
/// @param height height of the box, starting from 0
/// @return 1 if box too big 0 if succeed
bool _create_box_light(uint8_t screen_nbr, uint8_t width, uint8_t height) {
  uint8_t column, row = 0;

  if (width >= VGA_COL || height >= VGA_ROW) {
    return 1;
  }

  // Vertical
  for (uint8_t i = row; i <= height + row; ++i) {
    uint8_t current_char = CP437_BOX_DRAWINGS_LIGHT_VERTICAL;
    vga_printf(
        (vga_info){
            .screen = screen_nbr, .nocursor = true, .row = i, .column = column},
        "%C", current_char);
    vga_printf((vga_info){.screen = screen_nbr,
                          .nocursor = true,
                          .row = i,
                          .column = column + width},
               "%C", current_char);
  }

  // Horizontal
  for (uint8_t i = column; i <= width + column; ++i) {
    uint8_t current_char = CP437_BOX_DRAWINGS_LIGHT_HORIZONTAL;

    if (i == column)
      current_char = CP437_BOX_DRAWINGS_LIGHT_DOWN_AND_RIGHT;
    if (i == width)
      current_char = CP437_BOX_DRAWINGS_LIGHT_DOWN_AND_LEFT;
    vga_printf(
        (vga_info){
            .screen = screen_nbr, .nocursor = true, .row = row, .column = i},
        "%C", current_char);
    if (i == column)
      current_char = CP437_BOX_DRAWINGS_LIGHT_UP_AND_RIGHT;
    if (i == width)
      current_char = CP437_BOX_DRAWINGS_LIGHT_UP_AND_LEFT;
    vga_printf((vga_info){.screen = screen_nbr,
                          .nocursor = true,
                          .row = row + height,
                          .column = i},
               "%C", current_char);
  }

  return 0;
}

int cmd_test_box(uint8_t screen_nbr, int argc, char **argv) {
  vga_info screen_info = {.screen = screen_nbr};
  uint8_t width, height = 0;

  if (argc < 2) {
    vga_printf(screen_info, CMD_TEST_BOX_USAGE);
    return 1;
  }

  width = atoi(argv[0]);
  height = atoi(argv[1]);

  if (_create_box_light(screen_nbr, width, height)) {
    vga_printf(screen_info, "Error: width or height too big");
  }
  return 0;
}

int cmd_test_ipsum(uint8_t screen_nbr, int argc, char **argv) {
  (void)argc, (void)argv;
  vga_printf((vga_info){.screen = screen_nbr, .nowrapchar = true}, LOREM_IPSUM);
}

int cmd_test_vga(uint8_t screen_nbr, int argc, char **argv) {
  // clear the screen
  vga_screen_clear(screen_nbr);

  // test background
  vga_screen_fillbackground(screen_nbr, VGA_COLOR_BLUE);

  // test scroll
  vga_printf((vga_info){.screen = screen_nbr}, "This shouldn't be in view\n");
  vga_printf((vga_info){.screen = screen_nbr},
             "This REALLY shouldn't be in view\n");
  vga_printf(
      (vga_info){.screen = screen_nbr, .row = 24, .scrollattributes = true},
      "\n\n"); // put stuff above in history

  // test buffer upper and CP437 chars parameters
  vga_printf(
      (vga_info){
          .screen = screen_nbr, .row = 0, .column = 0, .setcursor = true},
      "%aX%c%C%c",
      (vga_attributes){.bg = VGA_COLOR_CYAN, .fg = VGA_COLOR_WHITE}, '4',
      CP437_HEART, '2');

  // test strings and escapes
  vga_printf(
      (vga_info){.screen = screen_nbr, .row = 3, .column = 70, .nowrap = false},
      "wrapping and %%s are: %s",
      "implemented and so is \\n \n in parameter strings");

  // test numbers
  vga_printf(
      (vga_info){
          .screen = screen_nbr, .row = 12, .column = 10, .nocursor = true},
      "int: %i hex:%x hexUP:%#X unsigned:%u octal: %#o binary: %#b", -42, 42,
      42, 42, 42, 42);
  vga_printf((vga_info){.screen = screen_nbr, .row = 24, .column = 78}, "%CY",
             CP437_YEN_SIGN);
}

int cmd_test_colors(uint8_t screen_nbr, int argc, char **argv) {
  for (uint8_t bg = 0; bg < 8; ++bg) {
    for (uint8_t fg = 0; fg < 16; ++fg) {
      vga_printf((vga_info){.screen = screen_nbr, .nowrap = true}, "%a%C",
                 (vga_attributes){.bg = bg, .fg = fg}, CP437_SMILEY);
    }
    vga_printf((vga_info){.screen = screen_nbr}, "\n");
  }
}

int cmd_test_cp437(uint8_t screen_nbr, int argc, char **argv) {
  uint8_t cp437[255] = {0};

  for (uint8_t c = 1; c < 255; ++c)
    cp437[c] = c;
  vga_printf((vga_info){.screen = screen_nbr, .nowrapchar = true}, "%C%S", 0,
             cp437 + 1);
}
