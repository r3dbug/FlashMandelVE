
#ifndef SYMMETRY_H
#define SYMMETRY_H

extern ULONG (*OriginalDrawFractal) (struct Window *,const LONG,const LONG,const LONG,const LONG);

void DisableSymmetry(void);
ULONG DrawFractalUsingSymmetry(struct Window *, const LONG,const LONG,const LONG,const LONG);
void EnableSymmetry(void);

#endif /* SYMMETRY_H */
