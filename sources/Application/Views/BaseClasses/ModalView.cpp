#include "ModalView.h"

// Upper bound on a modal border run, in characters.
#define MODAL_MAX_LINE 256

ModalView::ModalView(View &v)
    : View(v.w_, v.viewData_), finished_(false), returnCode_(0){};

ModalView::~ModalView(){};

int ModalView::GetReturnCode() { return returnCode_; };

bool ModalView::IsFinished() { return finished_; };

void ModalView::EndModal(int returnCode) {
    returnCode_ = returnCode;
    finished_ = true;
};

void ModalView::ClearRect(int x, int y, int w, int h) {
    View::ClearRect(x + left_, y + top_, w, h);
}
void ModalView::DrawString(int x, int y, const char *txt,
                           GUITextProperties &props) {
    View::DrawString(x + left_, y + top_, txt, props);
};

void ModalView::SetWindow(int width, int height) {

    if (width > 36) {
        width = 36;
    };
    // Leave room for the border and the chrome above/below.
    if (height > View::gridHeight_ - 4) {
        height = View::gridHeight_ - 4;
    };

    left_ = View::gridWidth_ / 2 - width / 2;
    top_ = View::gridHeight_ / 3 - height / 2;
    if (top_ < 2) {
        top_ = 2;
    }
    ClearRect(-1, -1, width + 2, height + 2);

    SetColor(CD_BORDER);
    GUITextProperties props;
    props.invert_ = true;
    // Border run, at most one grid row wide.
    int lineLen = View::gridWidth_;
    if (lineLen > MODAL_MAX_LINE) {
        lineLen = MODAL_MAX_LINE;
    }
    char line[MODAL_MAX_LINE + 1];
    memset(line, ' ', lineLen);
    line[lineLen] = 0;
    if (width + 4 < lineLen) {
        line[width + 4] = 0;
    }
    DrawString(-2, -2, line, props);
    DrawString(-2, height + 1, line, props);
    line[1] = 0;
    for (int i = 0; i < height + 2; i++) {
        DrawString(-2, i - 1, line, props);
        DrawString(width + 1, i - 1, line, props);
    }
};
