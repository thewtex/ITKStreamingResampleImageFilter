
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
::GenerateOutputInformation() 
{
  Superclass::GenerateOutputInformation();

  // get pointers to the input
  InputImagePointer  inputPtr  = 
    const_cast< TInputImage *>( this->GetInput() );
  // Get the output pointers
  OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }


  // will start withe the LargestPossibleRegion and trim it down
  OutputImageRegionType outputRegion = outputPtr->GetLargestPossibleRegion();
  OutputIndexType outputLargestIndex = outputRegion.GetIndex();
  OutputSizeType outputLargestSize   = outputRegion.GetSize();

  // indices for the bounding box
  OutputIndexType outputMinIndex;
  OutputIndexType outputMaxIndex;

  for( unsigned int i = 0; i < this->ImageDimension; i++ )
    {
    outputMinIndex[i] = outputLargestIndex[i] + outputLargestSize[i] - 1;
    outputMaxIndex[i] = outputLargestIndex[i];
    }

  typedef ContinuousIndex<TInterpolatorPrecisionType, ImageDimension> ContinuousIndexType;
  ContinuousIndexType outputContinuousIndex;

  InputIndexType inputIndex = inputPtr->GetRequestedRegion().GetIndex();
  InputSizeType  inputSize  = inputPtr->GetRequestedRegion().GetSize();
  InputIndexType inputBoundingIndex;

  // Define a few indices that will be used to translate from an output pixel
  // to an input pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel

  // unfortunately, m_Transform in the base class is private
  TransformPointerType transform = this->GetTransform();
  typename TransformType::InverseTransformBasePointer inverse_transform_base = transform->GetInverseTransform();
  typename TransformType::ConstPointer inverse_transform;
  try
    {
    inverse_transform = dynamic_cast< typename TransformType::Self * >( inverse_transform_base.GetPointer() );
    }
  catch( const std::bad_cast &e )
    {
    itkExceptionMacro( "Could not get inverse transform to determine output RequestedRegion." );
    }

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
	inputBoundingIndex[j] = inputIndex[j];
      else
	inputBoundingIndex[j] = inputIndex[j] + inputSize[j] - 1;
      ii = ii / 2;
      }
    inputPtr->TransformIndexToPhysicalPoint( inputBoundingIndex, inputPoint );
    outputPoint = inverse_transform->TransformPoint( inputPoint );
    outputPtr->TransformPhysicalPointToContinuousIndex( outputPoint, outputContinuousIndex );

    for( j = 0; j < ImageDimension; j++ )
      {
      tempIndexFloor = vcl_floor( outputContinuousIndex[j] ) ;
      if( tempIndexFloor < 0 )
	indexFloor = 0;
      else
	indexFloor = tempIndexFloor;
      if( indexFloor < outputMinIndex[j] )
	outputMinIndex[j] = indexFloor;

      tempIndexCeil  = vcl_ceil( outputContinuousIndex[j] ) ;
      if( tempIndexCeil < 0 )
	indexCeil = 0;
      else
	indexCeil = tempIndexCeil;
      if( indexCeil > outputMaxIndex[j] )
	outputMaxIndex[j] = indexCeil;
      }
    }

  OutputSizeType outputSize;
  for( j = 0; j < ImageDimension; j++ )
    {
    if( outputMinIndex[j] < outputLargestIndex[j] )
      outputMinIndex[j] = outputLargestIndex[j];

    if( outputMaxIndex[j] > static_cast< int >( outputLargestIndex[j] + outputLargestSize[j] - 1 ) )
      outputSize[j] = outputLargestIndex[j] + outputLargestSize[j] - outputMinIndex[j];
    else
      outputSize[j] = outputMaxIndex[j] - outputMinIndex[j] + 1;
    }
  outputRegion.SetIndex( outputMinIndex );
  outputRegion.SetSize( outputSize );

  outputPtr->SetRequestedRegion( outputRegion );
}


template < class TInputImage, class TOutputImage, class TInterpolatorPrecisionType >
void 
StreamingResampleImageFilter< TInputImage, TOutputImage,TInterpolatorPrecisionType >
::GenerateInputRequestedRegion( ) 
{
  Superclass::Superclass::GenerateInputRequestedRegion();

  // get pointers to the input
  InputImagePointer  inputPtr  = 
    const_cast< TInputImage *>( this->GetInput() );
  // Get the output pointers
  OutputImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }


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

  inputPtr->SetRequestedRegion( inputRegion );
}


}

#endif // __itkStreamingResampleImageFilter_txx
