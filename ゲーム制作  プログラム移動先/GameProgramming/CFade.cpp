#include "main.h"
#include <cassert>
#include <glew.h>
#include <gl/GL.h>
#include"CFade.h"
CFade* CFade::gInst = NULL;

CFade::CFade() :mFade(1.0f), mFadeType(FADE_IN), mbFade(true), mFadeSpeed(0.01f) {
    assert(!gInst);
    gInst = this;
}
void CFade::SetFade(eFadeMode type, float speed)
{
    if (gInst) {
        gInst->mFadeType = type;
        gInst->mbFade = true;
        gInst->mFadeSpeed = speed;
    }
}
bool CFade::IsFadeEnd()
{
    if (gInst) {
        return !gInst->mbFade;
    }
}
void CFade::Update() {
    if (mbFade) {
        switch (mFadeType) {
        case FADE_IN:
            mFade -= mFadeSpeed;
            if (mFade <= 0.0f) {
                mbFade = false;
                mFade = 0.0f;
            }
            break;
        case FADE_OUT:
            mFade += mFadeSpeed;
            if (mFade >= 1.0f){
                mbFade = false;
                mFade = 1.0f;
            }
            break;
        }
    }
}
void CFade::Render() {
    
    if (mFade > 0.0f) {//“§–¾‚Íˆ—‚ğ’Ê‚³‚È‚¢
        // •`‰æ
        glEnable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 1.0f, mFade);//Fİ’è@”’
        glBegin(GL_QUADS);
        glVertex2d(-1.0, 1.0);
        glVertex2d(-1.0, -1.0);
        glVertex2d(1.0, -1.0);
        glVertex2d(1.0, 1.0);
        glEnd();
        glDisable(GL_BLEND);

    }
}

