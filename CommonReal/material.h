#ifndef MATERIAL_H
#define MATERIAL_H

#include <QGLViewer/vec.h>
#include <qimage.h>
#include "color.h"

class Material
{
public :
  Material();

  enum TextureMode { MODULATE, BLEND, REPLACE };

  Color diffuseColor() const { return diffuseColor_; }
  Color diffuseColor(float u, float v) const;
  Color specularColor() const { return specularColor_; }
  Color reflectiveColor() const { return reflectiveColor_; }
  float specularCoefficient() const { return specularCoefficient_; }
  float textureScaleU() const { return textureScaleU_; }
  float textureScaleV() const { return textureScaleV_; }
  TextureMode textureMode() const { return textureMode_; }
  const QImage& texture() const { return texture_; }

  void setDiffuseColor(const Color& dc) { diffuseColor_ = dc; }
  void setSpecularColor(const Color& specularColor) { specularColor_ = specularColor; }
  void setReflectiveColor(const Color& reflectiveColor) { reflectiveColor_ = reflectiveColor; }
  void setSpecularCoefficient(float specularCoefficient) { specularCoefficient_ = specularCoefficient; }
  void setTexture(const QImage& texture) { texture_ = texture; }
  void loadTextureFromFile(const QString& fileName);
  void setTextureScale(float textureScale) { setTextureScaleU(textureScale); setTextureScaleV(textureScale); }
  void setTextureScaleU(float textureScaleU) { if (fabs(textureScaleU) > 1e-3) textureScaleU_ = textureScaleU; }
  void setTextureScaleV(float textureScaleV) { if (fabs(textureScaleV) > 1e-3) textureScaleV_ = textureScaleV; }
  void setTextureMode(TextureMode textureMode) { textureMode_ = textureMode; }

  void initFromDOMElement(const QDomElement& e);
  
private:
  Color diffuseColor_, specularColor_, reflectiveColor_;
  float specularCoefficient_, textureScaleU_, textureScaleV_;
  TextureMode textureMode_;
  QImage texture_;
};

#endif // MATERIAL_H
