#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-braces"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wpedantic"

#include <stdlib.h>
#include <stdio.h>

#define CLAY_IMPLEMENTATION
#include "lib/clay/clay.h"
#include "lib/clay/renderers/raylib/clay_renderer_raylib.c"

#include "hdr/infex.h"
#include "hdr/state.h"

#define FONT_ID_TITLE 0
#define FONT_TARGET_TITLE "res/font/Roboto-Regular.ttf"

/***************************************************************************************
 * CLAY INTERNALS
 */

uint64_t memory = 0;
Clay_Arena arena = { 0 };
Clay_Dimensions dimensions = { 0 };

Clay_RenderCommandArray (*interface_renderer)(void) = NULL;
Clay_RenderCommandArray render_commands = { 0 };

void clay_stderr(Clay_ErrorData error_data)
{
    static Clay_ErrorType t = 0;
    static uint64_t call = 0;
    fprintf(stderr, "CLAY: Error %d", error_data.errorType);
    if ((call > 0) && (error_data.errorType == t)) {
        fprintf(stderr, " (x%lu) ", call);
    }
    fprintf(stderr, "%s\n", error_data.errorText.chars);
    call++;
    t = error_data.errorType;
}

/***************************************************************************************
 *  INTERFACE ELEMENTS
 */

/*  MAIN MENU   ***********************************************************************/

#define NUM_MAINMENU_BUTTONS 1
enum MAINMENU_BUTTON {
    BUTTON_PLAY,
};

struct Button {
    Clay_String label;
};

struct Button mainmenu_buttons[1] = {
    { .label = CLAY_STRING("Play") }
};

Clay_LayoutConfig mainmenu_button_layout = {
    .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_FIXED(36)},
    .childAlignment = {
        .x = CLAY_ALIGN_X_CENTER,
        .y = CLAY_ALIGN_Y_CENTER
    }
};

Clay_RectangleElementConfig mainmenu_button_rectangle = {
    .color = { 64, 64, 128, 255 }
};

Clay_TextElementConfig mainmenu_button_text = {
    .fontId = FONT_ID_TITLE,
    .fontSize = 24,
    .textColor = { 0, 0, 0, 255 }
};


void render_mainmenu_title(void)
{
    CLAY(
        CLAY_ID("MainMenuTitle"),
        CLAY_RECTANGLE({ .color = { 0 } }),
        CLAY_LAYOUT({
            .sizing = {
                .width = CLAY_SIZING_GROW(0),
                .height = CLAY_SIZING_FIXED(60)
            },
            .childAlignment = {
                .x = CLAY_ALIGN_X_CENTER,
                .y = CLAY_ALIGN_Y_CENTER
            }
        })
    ) {
        CLAY_TEXT(
            CLAY_STRING("infex"),
            CLAY_TEXT_CONFIG({
                .fontId = FONT_ID_TITLE,
                .fontSize = 36,
                .textColor = { 0, 0, 0, 255 }
            })
        );
    }
}

/***************************************************************************************
 * INTERFACE FUNCTIONS
 */

void interface_initialise(void)
{
    /* MEMORY   */
    memory = Clay_MinMemorySize();
    arena = Clay_CreateArenaWithCapacityAndMemory(memory, malloc(memory));
    dimensions = (Clay_Dimensions) { GetScreenWidth(), GetScreenHeight() };

    /* FONTS    */
    Clay_SetMeasureTextFunction(Raylib_MeasureText);
    Raylib_fonts[FONT_ID_TITLE] = (Raylib_Font) {
        .font = LoadFontEx(FONT_TARGET_TITLE, 48, 0, 400),
        .fontId = FONT_ID_TITLE
    };
    SetTextureFilter(Raylib_fonts[FONT_ID_TITLE].font.texture, TEXTURE_FILTER_BILINEAR);

    /*  INIT    */
    Clay_Initialize(
        arena,
        dimensions,
        (Clay_ErrorHandler) { clay_stderr, 0 }
    );

}

Clay_RenderCommandArray interface_renderer_mainmenu()
{
    Clay_BeginLayout();

    CLAY(
        CLAY_ID("OuterContainer"),
        CLAY_RECTANGLE({ .color = { 200, 200, 200, 64 } }),
        CLAY_LAYOUT({
            .sizing = {
                .width = CLAY_SIZING_FIXED(screen_width()),
                .height = CLAY_SIZING_FIXED(screen_height())
            },
            .padding = { .left = 24 },
            .childGap = 24,
            .layoutDirection = CLAY_LEFT_TO_RIGHT
        })
    )

    {
        CLAY(
            CLAY_ID("MainMenuContainer"),
            CLAY_RECTANGLE({ .color = { 100, 100, 100, 255 } }),
            CLAY_LAYOUT({
                .sizing = {
                    .width = CLAY_SIZING_PERCENT(0.30),
                    .height = CLAY_SIZING_GROW(0),
                },
                .padding = { .left = 24, .right = 24, .top = 60, .bottom = 60 },
                .childGap = 12,
                .layoutDirection = CLAY_TOP_TO_BOTTOM
            })
        )

        {
            render_mainmenu_title();

            for (size_t i = 0; i < NUM_MAINMENU_BUTTONS; i++) {
                CLAY(
                    CLAY_LAYOUT(mainmenu_button_layout),
                    CLAY_RECTANGLE(mainmenu_button_rectangle)
                ) {
                    CLAY_TEXT(
                        mainmenu_buttons[i].label,
                        CLAY_TEXT_CONFIG(mainmenu_button_text)
                    );
                }
            }
        }
    }

    return Clay_EndLayout();
}

Clay_RenderCommandArray interface_renderer_fallback()
{
    Clay_BeginLayout();

    CLAY(
        CLAY_ID("OuterContainer"),
        CLAY_LAYOUT({
            .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) },
            .padding = { 24, 24 },
            .childGap = 16
        }),
        CLAY_RECTANGLE({ .color = { 200, 200, 200, 0 } })
    ) { }

    return Clay_EndLayout();
}

void interface_update(float dt)
{
    (void)dt;

    switch (screen_curr()) {
        case INFEX_SCREEN_MAINMENU:
            interface_renderer = interface_renderer_mainmenu;
            break;
        default:
            interface_renderer = interface_renderer_fallback;
            break;
    }
}

void interface_render(void)
{
    Clay_Raylib_Render(interface_renderer());
}

#pragma GCC diagnostic pop
