

#ifndef COLOR_H
#define COLOR_H

#include <qdom.h>
#include <qcolor.h>
#include <qstring.h>
#include <math.h>

#if QT_VERSION < 0x040000
# define qMin(a,b) QMIN(a,b)
#endif

/*! \brief A RGB color representation. Each color component ranges between 0.0 and 1.0. */

class Color
{
public:
  //! The internal data representation is public. One can use c.r, v.g, v.b.
  float r, g, b;

  /*! @name Setting Color values */
  //@{
  //! Default constructor. Default value is (0,0,0). Value range between 0.0 and 1.0.
  explicit Color() : r(0.0), g(0.0), b(0.0) {}

  //! Standard constructor with the red, green and blue values.
  explicit Color(float R, float G, float B) : r(R), g(G), b(B) {}

  //! QRgb to Color converter.
  explicit Color(QRgb rgb) : r(qRed(rgb)/255.0), g(qGreen(rgb)/255.0), b(qBlue(rgb)/255.0) {}

  //! Classical = operator
  Color& operator=(const Color& c)
  {
    r = c.r;   g = c.g;   b = c.b;
    return *this;
  }
  //@}
    
  /*! @name Access values */
  //@{
  //! Bracket operator, with a constant return value
  float operator[](int i) const { return (&r)[i]; }

  //! Bracket operator, returns an l-value
  float& operator[](int i) { return (&r)[i]; }

  //! Dereferencing operator that returns the memory address of the vector. You can hence use glColor3fv(myColor).
  operator const float*() const { return &r; };

  //! QRgb converter. Useful so that Color can be directly given as a parameter of QImage::setPixel().
  operator QRgb() const { return qRgb(qMin(int(255.0*r), 255), qMin(int(255.0*g), 255), qMin(int(255.0*b), 255)); };

  //! QString converter. Useful for debugging as in \c qWarning("Color is "+QString(color)) (Qt3).
  operator QString() const { return QString::number(int(255.0*r))+" "+QString::number(int(255.0*g))+" "+QString::number(int(255.0*b)); };
  //@}

  /*! @name Calculus */
  //@{
  //! Returns the component sum of the two colors.
  friend Color operator+(const Color &a, const Color &b)
  {
    return Color(a.r+b.r, a.g+b.g, a.b+b.b);
  }

  //! Adds \p c to the color.
  Color& operator+=(const Color &c)
  {
    r += c.r; g += c.g; b += c.b;
    return *this;
  }

  //! Color component by component product.
  friend Color operator*(const Color &a, const Color &b)
  {
    return Color(a.r*b.r, a.g*b.g, a.b*b.b);
  }

  //! Returns the product of the color with a scalar.
  friend Color operator*(const Color &c, float k)
  {
    return Color(c.r*k, c.g*k, c.b*k);
  }

  //! Returns the product of the color with a scalar.
  friend Color operator*(float k, const Color &c)
  {
    return c*k;
  }

  //! Returns the color divided by a scalar. Useful for Color averages. \attention Division by 0 is not checked.
  friend Color operator/(const Color &c, float k)
  {    
    return Color(c.r/k, c.g/k, c.b/k);
  }

  //! Returns the color divided by a scalar. Useful for Color averages. \attention Division by 0 is not checked.
  Color& operator/=(float k)
  {    
    r /= k;
    g /= k;
    b /= k;
    return *this;
  }

  //! Intensity of the Color. Norm of the RGB vector.
  float intensity() const { return (r+g+b)/3.0; };

  //! Clamp each color component in [0.0..1.0]. Adding colors may lead to values higher than 1.0.
  void clamp()
  {
    r = (r < 1.0) ? r : 1.0;
    g = (g < 1.0) ? g : 1.0;
    b = (b < 1.0) ? b : 1.0;
    r = (r > 0.0) ? r : 0.0;
    g = (g > 0.0) ? g : 0.0;
    b = (b > 0.0) ? b : 0.0;
  }
  //@}

  /*! @name XML representation */
  //@{
  void initFromDOMElement(const QDomElement& e)
  {
    r=e.attribute("red",   "0.0").toFloat();
    g=e.attribute("green", "0.0").toFloat();
    b=e.attribute("blue",  "0.0").toFloat();
  }
  //@}
};
  
#endif // COLOR_H
