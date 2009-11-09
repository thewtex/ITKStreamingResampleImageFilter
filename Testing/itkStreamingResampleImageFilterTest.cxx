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
#include "itkDifferenceImageFilter.h"
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

  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  transform->Scale( 0.8 );
  transform->Rotate( 0, 1, 0.2 );
  TransformType::OutputVectorType translation;
  translation[0] = 0.0;
  translation[1] = 3.0*spacing[1];
  translation[2] = 7.0*spacing[2];
  transform->SetTranslation( translation );

  streamedFilter->SetTransform( transform );
  streamedFilter->UseReferenceImageOn();
  streamedFilter->SetReferenceImage( reader->GetOutput() );
  streamedFilter->SetInput( reader->GetOutput() );
  notStreamedFilter->SetTransform( transform );
  notStreamedFilter->UseReferenceImageOn();
  notStreamedFilter->SetReferenceImage( reader->GetOutput() );
  notStreamedFilter->SetInput( reader->GetOutput() );

  writer->SetNumberOfStreamDivisions( 4 );
  try
    {
    writer->SetFileName( argv[2] );
    writer->SetInput( streamedFilter->GetOutput() );
    writer->Update();

    writer->SetFileName( argv[3] );
    writer->SetInput( notStreamedFilter->GetOutput() );
    writer->Update();

    typedef itk::Image< char, Dimension > DiffOutputType;
    ReaderType::Pointer testReader = ReaderType::New();
    testReader->SetFileName( argv[2] );
    ReaderType::Pointer baseLineReader = ReaderType::New();
    baseLineReader->SetFileName( argv[3] );
    typedef itk::DifferenceImageFilter< ImageType, DiffOutputType > DiffType;
    DiffType::Pointer diff = DiffType::New();
    diff->SetValidInput( baseLineReader->GetOutput() );
    diff->SetTestInput( testReader->GetOutput() );
    diff->SetToleranceRadius( 0 );
    diff->SetDifferenceThreshold( 2 );
    diff->UpdateLargestPossibleRegion();

    std::cout << "There were " << diff->GetNumberOfPixelsWithDifferences() << " pixels with differences." << std::endl;

    typedef itk::ImageFileWriter< DiffOutputType > DiffWriterType;
    DiffWriterType::Pointer diffwriter = DiffWriterType::New();
    diffwriter->SetInput( diff->GetOutput() );
    diffwriter->SetFileName( "StreamingResampleImageFilterDiff.mha" );
    diffwriter->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
