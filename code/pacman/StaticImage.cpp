#include "StaticImage.h"

StaticImage::StaticImage(const Texture2D* texture, const Rectangle& rect)
{
	img = texture;
	rc = rect;
}
StaticImage::StaticImage(const Texture2D* texture, const Rectangle& rect, bool t)
{
	img = texture;
	rc = rect;
	type = true;
}
StaticImage::~StaticImage()
{
}
void StaticImage::Draw(int x, int y) const
{
	if(!type) DrawTint(x, y, WHITE);
	else {
		y = y + 4;
		DrawTint(x, y, WHITE);
	}
}
void StaticImage::DrawTint(int x, int y, const Color& col) const
{
	DrawTextureRec(*img, rc, { (float)x, (float)y }, col);
}
void StaticImage::Release()
{
}