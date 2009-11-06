
#ifndef __itkStreamingResampleImageFilter_txx
#define __itkStreamingResampleImageFilter_txx

#include "itkStreamingResampleImageFilter.h"

namespace itk
{

template < class TInputImage, class TOutputImage, class TInterpolatorPrecisionType >
void 
StreamingResampleImageFilter< TInputImage, TOutputImage,TInterpolatorPrecisionType >
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}


template < class TInputImage, class TOutputImage, class TInterpolatorPrecisionType >
void 
StreamingResampleImageFilter< TInputImage, TOutputImage,TInterpolatorPrecisionType >
::GenerateInputRequestedRegion( ) 
{
}

}

#endif // __itkStreamingResampleImageFilter_txx
