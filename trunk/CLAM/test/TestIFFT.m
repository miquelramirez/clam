#** WARNING WARNING WARNING
# * Don't take this file as a good example of processing object test yet.
# * It will be re-made using a better design when XML load is available.
# */

function res = TestIFFT(Spectrum,Audio)
#  MaxRelativeError = 10e-2;
  MaxRelativeError = 0.02;
  full_spectrum = [ Spectrum fliplr(Spectrum(2:length(Spectrum)-1)) ];
  full_spectrum(length(Spectrum)+1:length(full_spectrum)) =  full_spectrum(length(Spectrum)+1:length(full_spectrum))  - 2*j*imag(full_spectrum(length(Spectrum)+1:length(full_spectrum)));
  good = ifft(full_spectrum);
  RelativeError = max(abs(good-Audio)./max(abs(good)));
  if RelativeError > MaxRelativeError
	printf(" Error too big: %f !",RelativeError); 
	res=1;
  else
	printf(" Err=%f ",RelativeError); 
	res=0;
  endif
endfunction

load IFFTTest.data

exit(TestIFFT(Spectrum,Audio))
