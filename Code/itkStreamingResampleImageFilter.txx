
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
  Superclass::Superclass::GenerateInputRequestedRegion();

  if ( !this->GetInput() || !this->GetOutput() )
    {
    return;
    }

  // get pointers to the input
  InputImagePointer  inputPtr  = 
    const_cast< TInputImage *>( this->GetInput() );
  // Get the output pointers
  OutputImagePointer outputPtr = this->GetOutput();

  // will start withe the LargestPossibleRegion and trim it down
  InputImageRegionType inputRegion = inputPtr->GetLargestPossibleRegion();
  InputIndexType inputLargestIndex = inputRegion.GetIndex();
  InputSizeType inputLargestSize   = inputRegion.GetSize();

  // indices for the bounding box
  InputIndexType inputMinIndex;
  InputIndexType inputMaxIndex;

  for( unsigned int i = 0; i < this->ImageDimension; i++ )
    {
    inputMinIndex[i] = inputLargestIndex[i] + inputLargestSize[i] - 1;
    inputMaxIndex[i] = inputLargestIndex[i];
    }

  typedef ContinuousIndex<TInterpolatorPrecisionType, ImageDimension> ContinuousIndexType;
  ContinuousIndexType inputContinuousIndex;

  OutputIndexType outputIndex = outputPtr->GetRequestedRegion().GetIndex();
  OutputSizeType  outputSize  = outputPtr->GetRequestedRegion().GetSize();
  OutputIndexType outputBoundingIndex;
  
  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel

  // unfortunately, m_Transform in the base class is private
  TransformPointerType transform = this->GetTransform();

  unsigned int boundingBoxPoints = 1;
  unsigned int i = 0;
  unsigned int ii = 0;
  int          tempIndexFloor;
  int	       tempIndexCeil;
  int indexFloor;
  int indexCeil;
  for( i = 0; i < this->ImageDimension; i++ )
    {
    boundingBoxPoints *= 2;
    }
  unsigned int j = 0;
  for( i = 0; i < boundingBoxPoints; i++ )
    {
    ii = i;
    for( j = 0; j < this->ImageDimension; j++ )
      {
      if( ii % 2 )
	outputBoundingIndex[j] = outputIndex[j];
      else
	outputBoundingIndex[j] = outputIndex[j] + outputSize[j] - 1;
      ii = ii / 2;
      }
    outputPtr->TransformIndexToPhysicalPoint( outputBoundingIndex, outputPoint );
    inputPoint = transform->TransformPoint( outputPoint );
    inputPtr->TransformPhysicalPointToContinuousIndex( inputPoint, inputContinuousIndex );

    for( j = 0; j < ImageDimension; j++ )
      {
      tempIndexFloor = vcl_floor( inputContinuousIndex[j] ) ;
      if( tempIndexFloor < 0 )
	indexFloor = 0;
      else
	indexFloor = tempIndexFloor;
      if( indexFloor < inputMinIndex[j] )
	inputMinIndex[j] = indexFloor;

      tempIndexCeil  = vcl_ceil( inputContinuousIndex[j] ) ;
      if( tempIndexCeil < 0 )
	indexCeil = 0;
      else
	indexCeil = tempIndexCeil;
      if( indexCeil > inputMaxIndex[j] )
	inputMaxIndex[j] = indexCeil;
      }
    }

  InputSizeType inputSize;
  for( j = 0; j < ImageDimension; j++ )
    {
    if( inputMinIndex[j] < inputLargestIndex[j] )
      inputMinIndex[j] = inputLargestIndex[j];

    if( inputMaxIndex[j] > static_cast< int >( inputLargestIndex[j] + inputLargestSize[j] - 1 ) )
      inputSize[j] = inputLargestIndex[j] + inputLargestSize[j] - inputMinIndex[j];
    else
      inputSize[j] = inputMaxIndex[j] - inputMinIndex[j] + 1;
    }
  inputRegion.SetIndex( inputMinIndex );
  inputRegion.SetSize( inputSize );

  inputPtr->SetRequestedRegion(inputRegion);
}

}

#endif // __itkStreamingResampleImageFilter_txx
