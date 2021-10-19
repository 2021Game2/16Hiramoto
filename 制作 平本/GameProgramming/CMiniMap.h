#ifndef CMINIMAP_H
#define CMINIMAP_H
#include"CTexture.h"
#include"CCharacter.h"
#include"CUtil.h"
#include"CPlayer.h"
#include"CEnemy.h"
#include"CEnemy2.h"
extern CTexture Texture;

class CMiniMap : public CCharacter{
public:
	CPlayer* mpPlayer;
	CEnemy* mpEnemy;
	CEnemy2* mpEnemy2;

	void Render() {
		if (mEnabled) {
			mpPlayer->mPosition.mX = Texture.width;
				//CRectangle::Render(Texture, 0, 48, 253, 213);
			
				//CRectangle::Render(Texture, 48, 95, 191, 144);
			

		}
	}
};

#endif

