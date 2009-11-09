/** 
 * @file itkStreamingResampleImageFilter.h
 * @brief Streaming version of the ResampleImageFilter
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @date 2009-11-05
 */

#ifndef __itkStreamingResampleImageFilter_h
#define __itkStreamingResampleImageFilter_h

#include "itkResampleImageFilter.h"

namespace itk
{

/** \class StreamingResampleImageFilter
 * \brief Streaming version of the ResampleImageFilter
 *
 * This provides a streaming version of the InsightToolkit class
 * ResampleImageFilter.
 * 
 * \warning This class should only be used when the transformed associated with the
 * object is linear or at least behaves linearly in the direction(s) that are
 * streamed.  Otherwise, the results may not be correct.
 * 
 * This class inherits from ResampleImageFilter, so the behavior is the same apart
 * from sending the RequestedRegion upstream instead of requiring the input to
 * update the LargestPossibleRegion for any RequestedRegion.
 */
template < class TInputImage, class TOutputImage,
          class TInterpolatorPrecisionType=double >
class ITK_EXPORT StreamingResampleImageFilter:
    public ResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
{
public:
  /** Standard class typedefs. */
  typedef StreamingResampleImageFilter                  Self;
  typedef ResampleImageFilter<TInputImage,TOutputImage, TInterpolatorPrecisionType>  Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  typedef TInputImage                             InputImageType;
  typedef TOutputImage                            OutputImageType;
  typedef typename InputImageType::Pointer        InputImagePointer;
  typedef typename InputImageType::ConstPointer   InputImageConstPointer;
  typedef typename OutputImageType::Pointer       OutputImagePointer;
  typedef typename InputImageType::RegionType     InputImageRegionType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingResampleImageFilter, ResampleImageFilter);

  /** Number of dimensions. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef typename TInputImage::IndexType  InputIndexType;
  typedef typename TOutputImage::IndexType OutputIndexType;

  typedef typename TInputImage::SizeType   InputSizeType;
  typedef typename Superclass::SizeType    OutputSizeType;

  /** Image point typedef. */
  typedef typename Superclass::PointType    PointType;

  typedef typename Superclass::TransformPointerType TransformPointerType;

protected:
  StreamingResampleImageFilter() {};
  ~StreamingResampleImageFilter() {};

  void PrintSelf( std::ostream& os, Indent indent ) const;

  /** We set the input requested region to the bounding box on the input
   * suggested by the corners of the output requested region, which is why
   * transform must behave linearly in the directions streamed. */
  virtual void GenerateInputRequestedRegion();

private:
  StreamingResampleImageFilter( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStreamingResampleImageFilter.txx"
#endif

#endif // __itkStreamingResampleImageFilter_h
