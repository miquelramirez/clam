

template <class T>
T max(T tX1, T tX2)
{
  T tX = tX1;
  if (tX < tX2)
    tX = tX2;
  return tX;
}

template <class T>
T min(T tX1, T tX2)
{
  T tX = tX1;
  if (tX > tX2)
    tX = tX2;
  return tX;
}