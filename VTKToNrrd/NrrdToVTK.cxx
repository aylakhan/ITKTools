#include "itkNrrdImageIO.h"
#include "itkVTKImageIO.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include <iostream>

int
main(int argc, char *argv[])
{
  if ( argc < 3 )
  {
    std::cerr << "Missing Parameters " << std::endl;
    std::cerr << "Usage: " << argv[0] << " nrrdFile vtkFile" << std::endl;
    return EXIT_FAILURE;
  }

  std::string nrrdPath = argv[1];
  std::string vtkPath = argv[2];

  const unsigned int OutputDimension = 3;
  const unsigned int InputDimension = 3;
  const unsigned int VectorComponents = 3;

  // from http://www.cmake.org/Wiki/Getting_Started_with_the_NRRD_Format
  typedef itk::Vector< double, VectorComponents> VectorType;
  typedef itk::Image< VectorType, OutputDimension > OutputImageType;
  typedef itk::Image< VectorType, InputDimension > InputImageType;

  itk::NrrdImageIO::Pointer nrrdImageIO = itk::NrrdImageIO::New();

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( nrrdPath );
  reader->SetImageIO( nrrdImageIO );

  try
  {
    reader->Update();
  }
  catch(itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }

  InputImageType::RegionType region = reader->GetOutput()->GetLargestPossibleRegion();
  InputImageType::IndexType start = region.GetIndex();
  InputImageType::SizeType size  = region.GetSize();

  if (region.GetImageDimension() < 3)
  {
    std::cerr << "Error: expecting 3D data." << std::endl;
    return EXIT_FAILURE;
  }

  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  itk::VTKImageIO::Pointer vtkImageIO = itk::VTKImageIO::New();
  writer->SetFileName( vtkPath );
  writer->SetImageIO( vtkImageIO );
  writer->SetInput( reader->GetOutput() );

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& e)
  {
    std::cerr << e << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
