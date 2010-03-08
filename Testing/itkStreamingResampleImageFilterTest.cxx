/** 
 * @file itkStreamingResampleImageFilterTest.cxx
 * @brief Test for StreamingResampleImageFilter
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @date 2009-11-05
 */

#include "itkTestMain.h"

void RegisterTests()
{
  REGISTER_TEST( itkStreamingResampleImageFilterTest );
}

#include "itkAffineTransform.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"

#include "itkStreamingResampleImageFilter.h"

int itkStreamingResampleImageFilterTest( int argc, char* argv[] )
{
  if( argc < 4 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFileStreamed outputImageFileNotStreamed ";
    std::cerr << std::endl;
    return EXIT_FAILURE;
    }

  typedef unsigned char PixelType;
  const unsigned int Dimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  typedef itk::AffineTransform< double, Dimension > TransformType;
  typedef itk::StreamingResampleImageFilter< ImageType, ImageType > StreamedFilterType;
  typedef itk::ResampleImageFilter< ImageType, ImageType > NotStreamedFilterType;
  typedef itk::ImageFileWriter< ImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  TransformType::Pointer transform = TransformType::New();
  StreamedFilterType::Pointer streamedFilter = StreamedFilterType::New();
  NotStreamedFilterType::Pointer notStreamedFilter = NotStreamedFilterType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  reader->UpdateOutputInformation();

  ImageType::SizeType    size    = reader->GetOutput()->GetLargestPossibleRegion().GetSize();
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  ImageType::PointType   origin  = reader->GetOutput()->GetOrigin();
  ImageType::DirectionType direction = reader->GetOutput()->GetDirection();

  transform->Scale( 0.8 );
  transform->Rotate( 0, 1, 0.2 );
  TransformType::OutputVectorType translation;
  translation[0] = 0.0;
  translation[1] = 3.0*spacing[1];
  translation[2] = 7.0*spacing[2];
  transform->SetTranslation( translation );

  size[2] = size[2] * 5;
  spacing[2] = spacing[2] / 5.;

  streamedFilter->SetTransform( transform );
  streamedFilter->SetOutputDirection( direction );
  streamedFilter->SetOutputOrigin( origin );
  streamedFilter->SetOutputSpacing( spacing );
  streamedFilter->SetSize( size );
  streamedFilter->SetInput( reader->GetOutput() );
  notStreamedFilter->SetTransform( transform );
  notStreamedFilter->SetOutputDirection( direction );
  notStreamedFilter->SetOutputOrigin( origin );
  notStreamedFilter->SetOutputSpacing( spacing );
  notStreamedFilter->SetSize( size );
  notStreamedFilter->SetInput( reader->GetOutput() );

  writer->SetNumberOfStreamDivisions( 8 );
  try
    {
    writer->SetFileName( argv[2] );
    writer->SetInput( streamedFilter->GetOutput() );
    writer->Update();

    writer->SetFileName( argv[3] );
    writer->SetInput( notStreamedFilter->GetOutput() );
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
