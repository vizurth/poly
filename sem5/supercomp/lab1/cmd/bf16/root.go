package main

import (
	"fmt"
	"strconv"

	"supercomp-lab1/internal/bf16"

	"github.com/spf13/cobra"
)

func newRootCmd() *cobra.Command {
	rootCmd := &cobra.Command{
		Use:   "bf16",
		Short: "Преобразование чисел в формат bfloat16 и обратно",
	}

	rootCmd.AddCommand(&cobra.Command{
		Use:                "to-bf16 <число>",
		Short:              "Преобразовать десятичное число в bfloat16",
		Args:               cobra.ExactArgs(1),
		DisableFlagParsing: true,
		RunE: func(cmd *cobra.Command, args []string) error {
			value, err := strconv.ParseFloat(args[0], 64)
			if err != nil {
				return fmt.Errorf("некорректное число: %w", err)
			}

			printBits(cmd, bf16.FloatToBits(value))
			return nil
		},
	})

	rootCmd.AddCommand(&cobra.Command{
		Use:                "from-bf16 <16-битная двоичная запись>",
		Short:              "Преобразовать 16 бит bfloat16 в десятичное число",
		Args:               cobra.ExactArgs(1),
		DisableFlagParsing: true,
		RunE: func(cmd *cobra.Command, args []string) error {
			fmt.Fprintf(cmd.OutOrStdout(), "%.9g\n\n", bf16.BitsToFloat(args[0]))
			return nil
		},
	})

	return rootCmd
}

func printBits(cmd *cobra.Command, bits uint16) {
	binary := bf16.BitsToBinary(bits)
	fmt.Fprintf(cmd.OutOrStdout(), "%s\n\nS = %s; E = %s; M = %s;\n", binary, binary[:1], binary[1:9], binary[9:])
}
